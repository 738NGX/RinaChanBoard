using System.Collections.Generic;
using UnityEngine;

public static class MediaTool
{
    public static readonly Dictionary<int, VoiceInfo> voiceInfos = DatabaseTool.BuildDatabase<VoiceInfo>("Database/VoiceDb");
    public static readonly Dictionary<int, MusicInfo> musicInfos = DatabaseTool.BuildDatabase<MusicInfo>("Database/MusicDb");
    public static readonly Dictionary<int, VideoInfo> videoInfos = DatabaseTool.BuildDatabase<VideoInfo>("Database/VideoDb");
    public static readonly Dictionary<int, TimeLine> voiceTimeLines = DatabaseTool.BuildVoiceTimelineDatabase();
    public static readonly Dictionary<int, TimeLine> musicTimeLines = DatabaseTool.BuildMusicTimelineDatabase();
    public static readonly Dictionary<int, TimeLine> videoTimeLines = DatabaseTool.BuildVideoTimelineDatabase();
    public static string ConvertFramesToTime(float frames)
    {
        int value = Mathf.FloorToInt(frames);
        int minutes = value / 600;
        int seconds = value % 600 / 10;
        int remain = value % 10;

        return $"{minutes:00}:{seconds:00}:{remain}";
    }
}