using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
using Newtonsoft.Json;

public class RinaChanDressManager : MonoBehaviour
{
    public Image Rina { get; private set; }
    private Image background;
    private Dropdown dressDropdown;     // 服装选择器
    private Dropdown bgDropdown;        // 背景选择器
    private readonly RinaChanDress rinaChanDress = new();

    private void Start()
    {
        Rina = GameObject.Find("Rina").GetComponent<Image>();
        background = GameObject.Find("bg").GetComponent<Image>();
        dressDropdown = transform.Find("Menu/Index/DressDropdown").GetComponent<Dropdown>();
        bgDropdown = transform.Find("Menu/Index/BGDropdown").GetComponent<Dropdown>();

        string path = Application.persistentDataPath + "/users/localsave.json";
        if (!File.Exists(path))
        {
            Rina.sprite = Resources.Load<Sprite>(rinaChanDress.SetDress(1, true));
            ChangeBackground(1);
        }
        else
        {
            string jsonData = File.ReadAllText(path);
            var savedDress = JsonConvert.DeserializeObject<RinaChanDress>(jsonData);
            Rina.sprite = Resources.Load<Sprite>(rinaChanDress.SetDress(savedDress.DressId, savedDress.WithBoard));
            background.sprite = Resources.Load<Sprite>(rinaChanDress.SetBackground(savedDress.BackgroundId));
            dressDropdown.value = savedDress.DressId - 1;
            bgDropdown.value = savedDress.BackgroundId - 1;
        }
    }
    public void OnSelectDress()
    {
        ChangeRinaChanDress(dressDropdown.value + 1);
    }
    public void OnSelectBG()
    {
        ChangeBackground(bgDropdown.value + 1);
    }
    // 改变璃奈立绘服装
    public void ChangeRinaChanDress(int id)
    {
        Rina.sprite = Resources.Load<Sprite>(rinaChanDress.ChangeDressId(id));
    }
    // 改变璃奈立绘是否带板
    public void ChangeRinaChanWithBoard()
    {
        Rina.sprite = Resources.Load<Sprite>(rinaChanDress.ChangeBoardStatus());
    }
    // 改变背景
    public void ChangeBackground(int id)
    {
        background.sprite = Resources.Load<Sprite>(rinaChanDress.ChangeBackground(id));
    }
    // 改变连接状态
    public void ChangeConnectionStatus()
    {
        Rina.sprite = Resources.Load<Sprite>(rinaChanDress.ChangeConnectionStatus());
    }
}
