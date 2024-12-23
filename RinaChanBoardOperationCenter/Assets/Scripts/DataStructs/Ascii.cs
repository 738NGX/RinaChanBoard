using System.Collections.Generic;

public readonly struct Ascii : IDatabase
{
    public int Id { get; }
    public string Symbol { get; }
    public List<List<int>> Content { get; }
    public Ascii(int id, string symbol, List<List<int>> content)
    {
        Id = id;
        Symbol = symbol;
        Content = content;
    }
}