using UnityEngine;
using UnityEngine.UI;

public class VoicePage : IPage
{
    // 组件
    private MainScene mainScene;                // 主场景
    private UdpServer udpServer;                // Udp服务
    private AudioSource voiceSource;            // 语音播放器
    private Transform voiceScrollViewContent;   // 语音滚动视图内容

    // 状态
    private bool isPlaying = false;             // 是否正在播放
    private int voiceFaceCount = -1;            // 表情计数器
    #pragma warning disable CS8632
    private TimeLine? voiceTimeLine;            // 当前语音时间线
    #pragma warning restore CS8632

    private void Start()
    {
        MainTransform = transform.Find("Voice");

        mainScene = GameObject.Find("Canvas").GetComponent<MainScene>();
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();

        voiceSource = GameObject.Find("VoiceSource").GetComponent<AudioSource>();
        voiceScrollViewContent = MainTransform.Find("VoiceScrollView/Viewport/Content");

        var template = voiceScrollViewContent.Find("Template");
        foreach (var voiceInfo in MediaTool.voiceInfos.Values)
        {
            var voiceItem = Instantiate(template, voiceScrollViewContent);
            voiceItem.name = "voice_" + voiceInfo.Id.ToString();
            voiceItem.Find("Text").GetComponent<Text>().text = voiceInfo.Content;
            voiceItem.Find("Button").GetComponent<Button>().onClick.AddListener(() => PlayVoice(voiceInfo.Id));
        }
        template.gameObject.SetActive(false);
    }
    private void Update()
    {
        if (isPlaying && !voiceSource.isPlaying)
        {
            isPlaying = false;
            voiceFaceCount = -1;
            mainScene.ChangePageMutex(false);
        }
        if (!isPlaying && voiceSource.isPlaying)
        {
            mainScene.ChangePageMutex(true);
            isPlaying = true;
        }
        if(isPlaying)
        {
            int frame = Mathf.FloorToInt(voiceSource.time * 10);

            if (voiceTimeLine != null)
            {
                var faceIndex = voiceTimeLine.GetFaceIndex(frame);
                if (faceIndex != null && faceIndex.Value > voiceFaceCount)
                {
                    voiceFaceCount = faceIndex.Value;
                    var face = voiceTimeLine.Content[faceIndex.Value].Face;
                    udpServer.LEDs.UpdateFace(face);
                    if (udpServer.SendLiteFace)
                        udpServer.UploadFaceLite(face);
                    else
                        udpServer.UploadFace();
                }
            }
        }
    }

    public void PlayVoice(int id)
    {
        if (isPlaying) return;
        if (MediaTool.voiceTimeLines.ContainsKey(id)) voiceTimeLine = MediaTool.voiceTimeLines[id];
        else voiceTimeLine = null;
        voiceSource.clip = Resources.Load<AudioClip>("Voice/voice_" + id);
        voiceSource.Play();
    }
}