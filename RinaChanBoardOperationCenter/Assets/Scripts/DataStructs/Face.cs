public readonly struct Face
{
    public int Leye { get; }
    public int Reye { get; }
    public int Mouth { get; }
    public int Cheek { get; }
    public Face(int leye, int reye, int mouth, int cheek)
    {
        Leye = leye;
        Reye = reye;
        Mouth = mouth;
        Cheek = cheek;
    }
    public string LiteFace => $"{Leye%100:X2}{Reye%100:X2}{Mouth%100:X2}{Cheek%100:X2}";
}