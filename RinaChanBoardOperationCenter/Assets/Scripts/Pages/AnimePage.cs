using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

public class AnimePage : IPage
{
    // 组件
    private MainScene mainScene;        // 主场景
    private UdpServer udpServer;        // Udp服务
    private VideoPlayer videoPlayer;    // 视频播放器
    private Slider videoSlider;         // 视频进度条
    private Text videoTimeValue;        // 视频时间戳
    private Text videoTimeLength;       // 视频时长

    // 状态
    private bool videoPlaying = false;  // 视频播放状态
    private VideoInfo currentVideo;     // 当前音乐信息
    private int videoFaceCount = -1;    // 表情计数器
#pragma warning disable CS8632
    private TimeLine? videoTimeLine;    // 当前音乐时间线
#pragma warning restore CS8632

    private void Start()
    {
        MainTransform = transform.Find("Anime");
        mainScene = GameObject.Find("Canvas").GetComponent<MainScene>();
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();

        videoPlayer = MainTransform.Find("VideoFrame").Find("VideoPlayer").GetComponent<VideoPlayer>();
        videoSlider = MainTransform.Find("VideoSlider").GetComponent<Slider>();
        videoTimeValue = MainTransform.Find("VideoTimeline").GetComponent<Text>();
        videoTimeLength = MainTransform.Find("VideoTimeLength").GetComponent<Text>();

        ChangeVideo(1);
    }
    private void Update()
    {
        if (videoPlaying)
        {
            int frame = Mathf.FloorToInt((float)(videoPlayer.time * 10));
            videoSlider.value = frame;

            if (videoTimeLine != null)
            {
                var faceIndex = videoTimeLine.GetFaceIndex(frame);
                if (faceIndex != null && faceIndex.Value > videoFaceCount)
                {
                    videoFaceCount = faceIndex.Value;
                    var face = videoTimeLine.Content[faceIndex.Value].Face;
                    udpServer.LEDs.UpdateFace(face);
                    if (udpServer.SendLiteFace)
                        udpServer.UploadFaceLite(face);
                    else
                        udpServer.UploadFace();
                }
            }
            videoTimeValue.text = MediaTool.ConvertFramesToTime(videoSlider.value);
            if (videoSlider.value >= videoSlider.maxValue - 2)
            {
                StopVideo();
                videoSlider.value = 0;
                videoPlayer.time = 0;
                videoFaceCount = -1;
                videoTimeValue.text = "00:00.0";
            }
        }
    }

    // 视频选择回调
    public void ChangeVideo(int id)
    {
        if (videoPlaying) StopVideo();

        currentVideo = MediaTool.videoInfos[id];
        if (MediaTool.videoTimeLines.ContainsKey(id)) videoTimeLine = MediaTool.videoTimeLines[id];
        else videoTimeLine = null;

        videoPlayer.clip = Resources.Load<VideoClip>("Video/video_" + currentVideo.Cover);
        videoSlider.value = 0;
        videoSlider.maxValue = Mathf.FloorToInt((float)(videoPlayer.clip.length * 10));
        videoTimeValue.text = "00:00.0";
        videoTimeLength.text = MediaTool.ConvertFramesToTime(videoSlider.maxValue);
        
        if(videoPlayer.isActiveAndEnabled)
        {
            PlayVideo();
            StopVideo();
        }
    }
    // 播放按钮回调
    public void OnPlayVideoButtonClicked()
    {
        if (videoPlaying) { StopVideo(); }
        else { PlayVideo(); }
    }
    // 视频进度条回调
    public void OnVideoSliderChange()
    {
        if (videoPlaying) return;

        videoPlayer.time = videoSlider.value / 10;
        videoFaceCount = -1;
        videoTimeValue.text = MediaTool.ConvertFramesToTime(videoSlider.value);
    }

    // 播放视频
    private void PlayVideo()
    {
        videoPlayer.Play();
        videoPlaying = true;
        mainScene.ChangePageMutex(true);
        MainTransform.Find("VideoPlayButton").GetComponent<Image>().sprite = Resources.Load<Sprite>("Image/Icons/icon_stop");
    }
    // 停止视频
    private void StopVideo()
    {
        videoPlayer.Pause();
        videoFaceCount = -1;
        videoPlaying = false;
        mainScene.ChangePageMutex(false);
        MainTransform.Find("VideoPlayButton").GetComponent<Image>().sprite = Resources.Load<Sprite>("Image/Icons/icon_play");
    }
}
