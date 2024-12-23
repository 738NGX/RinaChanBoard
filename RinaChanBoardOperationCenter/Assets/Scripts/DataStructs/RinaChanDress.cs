using System.IO;
using UnityEngine;
using Newtonsoft.Json;

public class RinaChanDress
{
    public int DressId { get; private set; }
    public int BackgroundId { get; private set; }
    public bool WithBoard { get; private set; }
    private bool isConnected = false;

    public RinaChanDress(int dressId = 1, int backgroundId = 1, bool withBoard = true)
    {
        DressId = dressId;
        BackgroundId = backgroundId;
        WithBoard = withBoard;
    }
    public string SetBackground(int id)
    {
        BackgroundId = id;
        return GetBgAddr();
    }
    public string ChangeBackground(int id)
    {
        BackgroundId = id;
        SaveData();
        return GetBgAddr();
    }
    public string SetDress(int id, bool wb)
    {
        DressId = id;
        WithBoard = wb;
        return GetImageAddr();
    }
    public string ChangeDressId(int id)
    {
        DressId = id;
        SaveData();
        return GetImageAddr();
    }
    public string ChangeBoardStatus()
    {
        WithBoard = !WithBoard;
        SaveData();
        return GetImageAddr();
    }
    public string ChangeConnectionStatus()
    {
        isConnected = !isConnected;
        return GetImageAddr();
    }
    private string GetImageAddr()
    {
        string end;
        if (isConnected) { end = "-c"; }
        else
        {
            end = WithBoard ? "-uc-wb" : "-uc-wob";
        }
        return "Image/Rina/rina-0" + DressId + end;
    }
    private string GetBgAddr()
    {
        return "Image/Background/bg-" + (BackgroundId < 10 ? "0" + BackgroundId : BackgroundId.ToString());
    }
    private void SaveData()
    {
        if (!File.Exists(Application.persistentDataPath + "/users"))
        {
            Directory.CreateDirectory(Application.persistentDataPath + "/users");
        }
        string jsonData = JsonConvert.SerializeObject(this, Formatting.Indented);
        File.WriteAllText(Application.persistentDataPath + "/users/localsave.json", jsonData);
    }
}
