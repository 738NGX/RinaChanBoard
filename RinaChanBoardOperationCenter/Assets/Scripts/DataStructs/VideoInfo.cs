public readonly struct VideoInfo : IDatabase
{
    public int Id { get; }
    public string Title { get; }
    public string Cover { get; }

    public VideoInfo(int id, string title, string cover)
    {
        Id = id;
        Title = title;
        Cover = cover;
    }
}