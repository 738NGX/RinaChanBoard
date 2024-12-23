using System.Collections.Generic;
using UnityEngine;
using Newtonsoft.Json;

public static class DatabaseTool
{
    public static Dictionary<int, T> BuildDatabase<T>(string src) where T : IDatabase
    {
        Dictionary<int, T> dict = new();

        string jsonData = Resources.Load<TextAsset>(src).text;
        var dataList = JsonConvert.DeserializeObject<List<T>>(jsonData);

        foreach (var data in dataList)
        {
            dict.Add(data.Id, data);
        }

        return dict;
    }

    public static Dictionary<string, Ascii> BuildAsciiDatabase()
    {
        Dictionary<string, Ascii> dict = new();

        string jsonData = Resources.Load<TextAsset>("Database/AsciiDb").text;
        var dataList = JsonConvert.DeserializeObject<List<Ascii>>(jsonData);

        foreach (var data in dataList)
        {
            dict.Add(data.Symbol, data);
        }

        return dict;
    }

    public static Dictionary<int, TimeLine> BuildVoiceTimelineDatabase()
    {
        Dictionary<int, TimeLine> dict = new();

        foreach (var voice in MediaTool.voiceInfos.Values)
        {
            string src = $"Database/VoiceTimeLineDb/voice_{voice.Id}";
            try
            {
                string jsonData = Resources.Load<TextAsset>(src).text;

                var data = JsonConvert.DeserializeObject<List<TimeLineUnit>>(jsonData);
                dict.Add(voice.Id, new TimeLine(voice.Id, data));
            }
            catch
            {
                Debug.Log($"Failed to load timeline data for {voice.Content}.");
            }
        }

        return dict;
    }
    
    public static Dictionary<int, TimeLine> BuildMusicTimelineDatabase()
    {
        Dictionary<int, TimeLine> dict = new();

        foreach (var music in MediaTool.musicInfos.Values)
        {
            string src = $"Database/MusicTimeLineDb/{music.Cover}";
            try
            {
                string jsonData = Resources.Load<TextAsset>(src).text;

                var data = JsonConvert.DeserializeObject<List<TimeLineUnit>>(jsonData);
                dict.Add(music.Id, new TimeLine(music.Id, data));
            }
            catch
            {
                Debug.Log($"Failed to load timeline data for {music.Title}.");
            }
        }

        return dict;
    }

    public static Dictionary<int, TimeLine> BuildVideoTimelineDatabase()
    {
        Dictionary<int, TimeLine> dict = new();

        foreach (var anime in MediaTool.videoInfos.Values)
        {
            string src = $"Database/VideoTimeLineDb/{anime.Cover}";
            try
            {
                string jsonData = Resources.Load<TextAsset>(src).text;

                var data = JsonConvert.DeserializeObject<List<TimeLineUnit>>(jsonData);
                dict.Add(anime.Id, new TimeLine(anime.Id, data));
            }
            catch
            {
                Debug.Log($"Failed to load timeline data for {anime.Title}.");
            }
        }

        return dict;
    }
}