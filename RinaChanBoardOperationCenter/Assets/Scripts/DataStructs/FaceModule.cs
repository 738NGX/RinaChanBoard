using System.Collections.Generic;

public readonly struct FaceModule : IDatabase
{
    public int Id { get; }
    public List<List<int>> Content { get; }
    public FaceModule(int id, List<List<int>> content)
    {
        Id = id;
        Content = content;
    }
}