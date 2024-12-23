public readonly struct MusicInfo : IDatabase
{
    public int Id { get; }
    public string Title { get; }
    public string Cover { get; }
    public string Artist { get; }
    public string Description { get; }

    public MusicInfo(int id, string title, string cover, string artist, string description)
    {
        Id = id;
        Title = title;
        Cover = cover;
        Artist = artist;
        Description = description;
    }
}