using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class IndexPage : IPage
{
    // 组件
    private UdpServer udpServer;
    private Text udpServerButtonText;   // 连接按钮显示文字
    private Slider brightSlider;        // 亮度控制条
    private Slider rSlider;             // 红色控制条
    private Slider gSlider;             // 绿色控制条
    private Slider bSlider;             // 蓝色控制条
    private Transform childColorGroup;  // 预设颜色子组

    // 状态
    private int parentColorId = 3;      // 预设颜色父组ID
    
    private void Start()
    {
        MainTransform = transform.Find("Index");

        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
        
        udpServerButtonText = MainTransform.Find("UdpServerButton").Find("UdpServerButtonText").GetComponent<Text>();
        brightSlider = MainTransform.Find("BrightSlider").GetComponent<Slider>();
        rSlider = MainTransform.Find("RSlider").GetComponent<Slider>();
        gSlider = MainTransform.Find("GSlider").GetComponent<Slider>();
        bSlider = MainTransform.Find("BSlider").GetComponent<Slider>();
        childColorGroup = MainTransform.Find("ChildColorGroup").transform;
    }
    private void Update()
    {
        // 监听亮度数值,改变时触发回调进行同步,受互斥锁保护
        if (udpServer.Mutex && BrightnessSliderMap(brightSlider.value) != udpServer.LEDs.Brightness)
        {
            brightSlider.value = udpServer.LEDs.Brightness switch
            {
                16 => 0,
                32 => 1,
                64 => 2,
                128 => 3,
                _ => 0
            };
            MainTransform.Find("BrightText").GetComponent<Text>().text = brightSlider.value switch
            {
                0 => "正常",
                1 => "輝く",
                2 => "眩しい",
                3 => "光害",
                _ => "正常"
            };
            udpServer.Mutex = false;
        }
        // 监听颜色更改,改变时触发回调进行同步
        if (udpServer.LEDs.WaitColorUpdate)
        {
            SetColor(udpServer.LEDs.LedOnColor);
        }
    }

    // 设置连接按钮文字
    public void SetUdpServerButtonText(string text)
    {
        udpServerButtonText.text = text;
    }
    // 改变LED亮度
    public void ChangeBrightness()
    {
        udpServer.LEDs.Brightness = BrightnessSliderMap(brightSlider.value);
        MainTransform.Find("BrightText").GetComponent<Text>().text = brightSlider.value switch
        {
            0 => "正常",
            1 => "輝く",
            2 => "眩しい",
            3 => "光害",
            _ => "正常"
        };
    }
    // 颜色滚动条改变回调
    public void ChangeColorSlider()
    {
        var newColor = new Color(rSlider.value / 255, gSlider.value / 255, bSlider.value / 255);
        SetColor(newColor);
    }
    // 选择预设颜色父组
    public void ChangeParentColorGroup(int id)
    {
        parentColorId = id;
        var parentColor = id switch
        {
            0 => LedTool.HexToColor("f971d4"),
            1 => LedTool.HexToColor("e4007f"),
            2 => LedTool.HexToColor("00a1e8"),
            3 => LedTool.HexToColor("f8b656"),
            4 => LedTool.HexToColor("a5469b"),
            5 => LedTool.HexToColor("fb8a9b"),
            _ => LedTool.HexToColor("f971d4")
        };
        SetColor(parentColor);

        for (int i = 0; i < 16; i++)
        {
            if (i >= LedTool.colorValue[id].Count)
            {
                childColorGroup.GetChild(i).gameObject.SetActive(false);
            }
            else
            {
                var btn = childColorGroup.GetChild(i);
                btn.gameObject.SetActive(true);
                btn.GetComponent<Image>().color = LedTool.HexToColor(LedTool.colorValue[id][i]);
                btn.GetChild(0).GetComponent<Text>().text = LedTool.colorName[id][i];
            }
        }
    }
    // 选择预设颜色子组
    public void ChangeChildColorGroup(int id)
    {
        var childColor = LedTool.HexToColor(LedTool.colorValue[parentColorId][id]);
        SetColor(childColor);
    }

    // 亮度调到亮度值的映射
    private int BrightnessSliderMap(float value)
    {
        return value switch
        {
            0 => 16,
            1 => 32,
            2 => 64,
            3 => 128,
            _ => 16
        };
    }
    // 颜色调整
    private void SetColor(Color color)
    {
        MainTransform.Find("ColorText").GetComponent<Text>().text = "#" + LedTool.ColorToHex(color);

        rSlider.value = Mathf.RoundToInt(color.r * 255);
        gSlider.value = Mathf.RoundToInt(color.g * 255);
        bSlider.value = Mathf.RoundToInt(color.b * 255);
        rSlider.transform.Find("value").GetComponent<Text>().text = "R: " + rSlider.value;
        gSlider.transform.Find("value").GetComponent<Text>().text = "G: " + gSlider.value;
        bSlider.transform.Find("value").GetComponent<Text>().text = "B: " + bSlider.value;

        udpServer.LEDs.UpdateColor(color);
    }
}
