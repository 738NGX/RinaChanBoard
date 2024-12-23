using System.Collections.Generic;

public class TimeLine : IDatabase
{
    public int Id { get; }
    public List<TimeLineUnit> Content { get; }
    private readonly Dictionary<int,int> contentHash = new();
    public TimeLine(int id, List<TimeLineUnit> content)
    {
        Id = id;
        Content = content;
        foreach (var unit in content)
        {
            contentHash[unit.Frame] = content.IndexOf(unit);
        }
    }
    public int? GetFaceIndex(int frame)
    {
        if(contentHash.TryGetValue(frame, out var index))
        {
            return index;
        }
        return null;
    }
}