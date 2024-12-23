using UnityEngine;
using UnityEngine.UI;

public class MusicPage : IPage
{
    // 组件
    private MainScene mainScene;        // 主场景
    private UdpServer udpServer;        // Udp服务
    private AudioSource musicSource;    // 音乐播放器
    private Slider musicSlider;         // 音乐进度条
    private Text musicTimeValue;        // 音乐时间戳
    private Text musicTimeLength;       // 音乐时长

    // 状态
    private bool musicPlaying = false;  // 音乐播放状态
    private MusicInfo currentMusic;     // 当前音乐信息
    private int musicFaceCount = -1;    // 表情计数器
    #pragma warning disable CS8632
    private TimeLine? musicTimeLine;    // 当前音乐时间线
    #pragma warning restore CS8632

    private void Start()
    {
        MainTransform = transform.Find("Music");

        mainScene = GameObject.Find("Canvas").GetComponent<MainScene>();
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
        
        musicSource = GameObject.Find("MusicSource").GetComponent<AudioSource>();
        musicSlider = MainTransform.Find("MusicSlider").GetComponent<Slider>();
        musicTimeValue = MainTransform.Find("MusicTimeline").GetComponent<Text>();
        musicTimeLength = MainTransform.Find("MusicTimeLength").GetComponent<Text>();

        ChangeMusic(200001);
    }
    private void Update()
    {
        // 正在播放歌曲时执行的操作
        if (musicPlaying)
        {
            int frame = Mathf.FloorToInt(musicSource.time * 10);
            musicSlider.value = frame;

            if (musicTimeLine != null)
            {
                var faceIndex = musicTimeLine.GetFaceIndex(frame);
                if (faceIndex != null && faceIndex.Value > musicFaceCount)
                {
                    musicFaceCount = faceIndex.Value;
                    var face = musicTimeLine.Content[faceIndex.Value].Face;
                    udpServer.LEDs.UpdateFace(face);
                    if (udpServer.SendLiteFace)
                        udpServer.UploadFaceLite(face);
                    else
                        udpServer.UploadFace();
                }
            }
            musicTimeValue.text = MediaTool.ConvertFramesToTime(musicSlider.value);
            if (musicSlider.value >= musicSlider.maxValue)
            {
                StopMusic();
                musicSlider.value = 0;
                musicSource.time = 0;
                musicFaceCount = -1;
                musicTimeValue.text = "00:00.0";
            }
        }
    }
    
    // 歌曲选择回调
    public void ChangeMusic(int id)
    {
        if (musicPlaying) StopMusic();

        currentMusic = MediaTool.musicInfos[id];
        if (MediaTool.musicTimeLines.ContainsKey(id)) musicTimeLine = MediaTool.musicTimeLines[id];
        else musicTimeLine = null;

        var musicCover = MainTransform.Find("MusicCover").gameObject.GetComponent<Image>();
        var musicTitle = MainTransform.Find("MusicTitle").gameObject.GetComponent<Text>();
        var musicArtist = MainTransform.Find("MusicArtist").gameObject.GetComponent<Text>();
        var musicDescription = MainTransform.Find("MusicDescription").gameObject.GetComponent<Text>();

        musicCover.sprite = Resources.Load<Sprite>("Image/Music/music_" + currentMusic.Cover);
        musicTitle.text = currentMusic.Title;
        musicArtist.text = currentMusic.Artist;
        musicDescription.text = currentMusic.Description;

        if(musicTimeLine is not null)
        {
            // musicDescription.text += $"(表情数量：{musicTimeLine.Content.Count})";
        }

        musicSource.clip = Resources.Load<AudioClip>("Music/music_" + currentMusic.Cover);
        musicSlider.value = 0;
        musicSlider.maxValue = Mathf.FloorToInt(musicSource.clip.length * 10);
        musicTimeValue.text = "00:00.0";
        musicTimeLength.text = MediaTool.ConvertFramesToTime(musicSlider.maxValue);
    }
    // 播放按钮回调
    public void OnPlayMusicButtonClicked()
    {
        if (musicPlaying) { StopMusic(); }
        else { PlayMusic(); }
    }
    // 歌曲进度条回调
    public void OnMusicSliderChange()
    {
        if (musicPlaying) return;

        musicSource.time = musicSlider.value / 10;
        musicFaceCount = -1;
        musicTimeValue.text = MediaTool.ConvertFramesToTime(musicSlider.value);
    }
    
    // 播放歌曲
    private void PlayMusic()
    {
        musicSource.Play();
        musicPlaying = true;
        mainScene.ChangePageMutex(true);
        MainTransform.Find("MusicPlayButton").GetComponent<Image>().sprite = Resources.Load<Sprite>("Image/Icons/icon_stop");
    }
    // 停止歌曲
    private void StopMusic()
    {
        musicSource.Pause();
        musicFaceCount = -1;
        musicPlaying = false;
        mainScene.ChangePageMutex(false);
        MainTransform.Find("MusicPlayButton").GetComponent<Image>().sprite = Resources.Load<Sprite>("Image/Icons/icon_play");
    }
}
