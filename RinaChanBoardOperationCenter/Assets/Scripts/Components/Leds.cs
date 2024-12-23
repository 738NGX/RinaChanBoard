using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;

public class Leds : MonoBehaviour
{
    public bool CanCustom { get; set; } = false;
    public bool WaitColorUpdate { get; set; } = false;
    public int Brightness { get; set; } = 16;
    public Color LedOnColor { get; private set; } = new(249f / 255, 113f / 255, 212f / 255);
    private Color LedOffColor { get; } = new(1f, 1f, 1f, 0f);
    private readonly List<Image> leds = new();
    private readonly StringBuilder faceString = new(new string('0', 288));

    public void UpdateColor(Color color)
    {
        LedOnColor = color;
        foreach (var led in leds)
        {
            if (led.color.a > 0f) { led.color = LedOnColor; }
        }
    }
    public void UpdateFace(Face face)
    {
        UpdateCheek(face.Cheek);
        UpdateLeftEye(face.Leye);
        UpdateRightEye(face.Reye);
        UpdateMouth(face.Mouth);
    }
    public void ClearFace()
    {
        faceString.Clear();
        faceString.Append(new string('0', 288));
        foreach (var led in leds)
        {
            led.color = LedOffColor;
        }
    }
    public void UpdateLeftEye(int id)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                UpdateFaceModule(id, i, j, 0, 0);
            }
        }
    }
    public void UpdateRightEye(int id)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 10; j < 18; j++)
            {
                UpdateFaceModule(id, i, j, 0, 10);
            }
        }
    }
    public void UpdateMouth(int id)
    {
        for (int i = 8; i < 16; i++)
        {
            for (int j = 5; j < 13; j++)
            {
                UpdateFaceModule(id, i, j, 8, 5);
            }
        }
    }
    public void UpdateCheek(int id)
    {
        for (int i = 8; i < 12; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int index = LedTool.ledMap[i][j];
                if (index == -1) continue;

                if (LedTool.faceModules[id].Content[i - 8][3 - j] == 1)
                {
                    leds[index].color = LedOnColor;
                    faceString[i * 18 + j] = '1';
                }
                else
                {
                    leds[index].color = LedOffColor;
                    faceString[i * 18 + j] = '0';
                }
            }
            for (int j = 14; j < 18; j++)
            {
                UpdateFaceModule(id, i, j, 8, 14);
            }
        }
    }
    public void SwitchLed(int index)
    {
        if (!CanCustom) return;
        var led = leds[index];
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                int ledIndex = LedTool.ledMap[i][j];
                if (ledIndex == index)
                {
                    if (led.color.a == 0.0f)
                    {
                        led.color = LedOnColor;
                        faceString[i * 18 + j] = '1';
                    }
                    else
                    {
                        led.color = LedOffColor;
                        faceString[i * 18 + j] = '0';
                    }
                }
            }
        }
    }
    public void UpdateLedsByString(string hexString)
    {
        string binaryString = LedTool.HexToBinary(hexString);
        faceString.Clear();
        faceString.Append(binaryString);

        for (int i = 0; i < binaryString.Length; i++)
        {
            int index = LedTool.ledMap[i / 18][i % 18];

            if (index == -1)
            {
                continue;
            }

            var led = leds[index];

            if (binaryString[i] == '1')
            {
                if (led.color.a == 0.0f)
                {
                    led.color = LedOnColor;
                }
            }
            else
            {
                if (led.color.a == 1.0f)
                {
                    led.color = LedOffColor;
                }
            }
        }
    }
    public string EncodeFace(int from = 0, int to = 288)
    {
        return LedTool.BinaryToHex(faceString.ToString()[from..to]);
    }

    private void Start()
    {
        for (int i = 0; i < transform.childCount; i++)
        {
            var led = transform.GetChild(i);
            var ledImage = led.GetComponent<Image>();
            var ledButton = led.GetComponent<Button>();
            leds.Add(ledImage);

            int index = i;
            ledButton.onClick.AddListener(() =>
            {
                SwitchLed(index);
            });
        }
    }
    private void UpdateFaceModule(int id, int i, int j, int iStart, int jStart)
    {
        int index = LedTool.ledMap[i][j];
        if (index == -1) return;

        if (LedTool.faceModules[id].Content[i - iStart][j - jStart] == 1)
        {
            leds[index].color = LedOnColor;
            faceString[i * 18 + j] = '1';
        }
        else
        {
            leds[index].color = LedOffColor;
            faceString[i * 18 + j] = '0';
        }
    }
}
