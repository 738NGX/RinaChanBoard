public readonly struct VoiceInfo : IDatabase
{
    public int Id { get; }
    public string Content { get; }

    public VoiceInfo(int id, string content)
    {
        Id = id;
        Content = content;
    }
}