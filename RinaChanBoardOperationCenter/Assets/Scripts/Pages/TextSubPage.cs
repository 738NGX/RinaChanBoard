using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Linq;
using TMPro;

public class TextSubPage : IPage
{
    private Slider textSpeedSlider;     // 文本速度滑块
    private TMP_InputField textInput;   // 文本输入框
    private UdpServer udpServer;        // UDP 服务器

    private bool isPlaying = false;     // 是否正在播放
    private bool stopPlay = false;      // 是否停止播放
    private string inputString = "";    // 输入的字符串
    private float speed = 0.1f;         // 文字滚动速度
    private readonly List<Ascii> asciiList = new();
    private readonly List<List<int>> ledDisplay = new() {
        new(), new(), new(), new(), new(), new(), new()
    };

    private void Start()
    {
        MainTransform = transform.Find("Text");
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
        textSpeedSlider = MainTransform.Find("TextSpeedSlider").GetComponent<Slider>();
        textInput = MainTransform.Find("TextInput").GetComponent<TMP_InputField>();
    }

    public void OnChangeSpeed()
    {
        if (isPlaying)
        {
            textSpeedSlider.value = speed * 10;
            return;
        }
        speed = textSpeedSlider.value / 10f;
        string speedStr = speed.ToString("0.0") + " s/frame";
        MainTransform.Find("TextSpeed").GetComponent<Text>().text = speedStr;
    }
    public void OnInputString()
    {
        if (isPlaying)
        {
            textInput.text = inputString;
            return;
        }
        inputString = textInput.text.Length > 30 ? textInput.text[..30] : textInput.text;
        textInput.text = inputString;
    }
    public void SendStr()
    {
        if (inputString.Length == 0) return;
        UpdateAsciiStr(inputString);
    }
    public void StopPlay()
    {
        if (!isPlaying) return;
        stopPlay = true;
    }
    public void UpdateAsciiStr(string str)
    {
        if (isPlaying) return;
        InitInput(str);
        StartCoroutine(Play());
    }

    private void InitInput(string str)
    {
        asciiList.Clear();
        foreach (var row in ledDisplay)
        {
            row.Clear();
            row.AddRange(Enumerable.Repeat(0, 18));
        }

        foreach (var c in str)
        {
            if (LedTool.asciis.TryGetValue(c.ToString(), out var ascii))
            {
                asciiList.Add(ascii);
            }
        }

        for (int i = 0; i < 7; i++)
        {
            foreach (var ascii in asciiList)
            {
                ledDisplay[i].AddRange(ascii.Content[i]);
                ledDisplay[i].Add(0);
            }
        }

        foreach (var row in ledDisplay)
        {
            row.AddRange(Enumerable.Repeat(0, 18));
        }
    }
    private string GetFrame(int frame)
    {
        if (frame < 0 || frame + 18 >= ledDisplay[0].Count)
        {
            return new string('0', 72);
        }

        string frameStr = new('0', 72);
        for (int i = 0; i < 7; i++)
        {
            for (int j = frame; j < frame + 18; j++)
            {
                frameStr += ledDisplay[i][j];
            }
        }
        frameStr += new string('0', 90);

        return LedTool.BinaryToHex(frameStr);
    }
    private IEnumerator<WaitForSeconds> Play()
    {
        isPlaying = true;
        transform.parent.GetComponent<MorePage>().ChangePageMutex(true);

        for (int i = 0; i < ledDisplay[0].Count - 18; i++)
        {
            string frame = GetFrame(i);
            udpServer.LEDs.UpdateLedsByString(frame);
            if (udpServer.SendLiteFace)
                udpServer.UploadFace(72, 200);
            else
                udpServer.UploadFace();

            if (stopPlay)
            {
                stopPlay = false;
                udpServer.LEDs.ClearFace();
                break;
            }
            yield return new WaitForSeconds(speed);
        }
        udpServer.LEDs.UpdateFace(new(101, 201, 313, 400));
        if (udpServer.SendLiteFace)
            udpServer.UploadFaceLite(new(101, 201, 313, 400));
        else
            udpServer.UploadFace();
        transform.parent.GetComponent<MorePage>().ChangePageMutex(false);
        isPlaying = false;
    }
}
