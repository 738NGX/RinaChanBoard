using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

public class LipSyncSubPage : IPage
{
    private UdpServer udpServer;
    private Transform rinaLipSyncTemplate;
    private Transform rinaLipSync;
    private RinaLipSync.RinaLipSyncFaceManager faceManager;
    private Text controlButtonText;
    private Slider micSensitivitySlider;
    private Slider syncSpeedSlider;
    private Dropdown profileDropdown;

    private int currentProfileIndex = 0;
    private List<RinaLipSync.Profile> profiles;
    private bool isSyncing = false;

    private void Start()
    {
        MainTransform = transform.Find("LipSync");
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();

        rinaLipSyncTemplate = MainTransform.Find("RinaLipSync");
        faceManager = rinaLipSyncTemplate.GetComponent<RinaLipSync.RinaLipSyncFaceManager>();

        controlButtonText = MainTransform.Find("ControlButton/Text").GetComponent<Text>();
        micSensitivitySlider = MainTransform.Find("MicSensitivitySlider").GetComponent<Slider>();
        syncSpeedSlider = MainTransform.Find("SyncSpeedSlider").GetComponent<Slider>();
        profileDropdown = MainTransform.Find("ProfileDropdown").GetComponent<Dropdown>();

        profiles = new(){
            Resources.Load<RinaLipSync.Profile>("LipSyncProfile/LipSync_Standard"),
            Resources.Load<RinaLipSync.Profile>("LipSyncProfile/LipSync_Male"),
            Resources.Load<RinaLipSync.Profile>("LipSyncProfile/LipSync_Female"),
            Resources.Load<RinaLipSync.Profile>("LipSyncProfile/LipSync_Anime"),
        };
    }

    public void OnControlButtonClick()
    {
        if (isSyncing)
        {
            Destroy(rinaLipSync.gameObject);
            transform.parent.GetComponent<MorePage>().ChangePageMutex(false);
            controlButtonText.text = "开始口型同步";
            udpServer.LEDs.UpdateFace(new(101, 201, 302, 400));
            udpServer.UploadFace();
            isSyncing = false;
        }
        else
        {
            udpServer.LEDs.UpdateFace(new(101, 201, 302, 400));
            udpServer.UploadFace();
            transform.parent.GetComponent<MorePage>().ChangePageMutex(true);
            controlButtonText.text = "结束口型同步";
            rinaLipSync = Instantiate(rinaLipSyncTemplate, MainTransform);
            rinaLipSync.gameObject.SetActive(true);
            isSyncing = true;
        }
    }

    public void OnMicSensitivityChange()
    {
        if(isSyncing)
        {
            micSensitivitySlider.value = faceManager.minVolume;
            return;
        }
        faceManager.minVolume = micSensitivitySlider.value;
    }

    public void OnSyncSpeedChange()
    {
        if(isSyncing)
        {
            syncSpeedSlider.value = faceManager.minDuration;
            return;
        }
        faceManager.minDuration = syncSpeedSlider.value;
    }

    public void OnProfileChange()
    {
        if(isSyncing)
        {
            profileDropdown.value = currentProfileIndex;
            return;
        }
        currentProfileIndex = profileDropdown.value;
        rinaLipSync.GetComponent<RinaLipSync.RinaLipSync>().profile = profiles[currentProfileIndex];
    }
}