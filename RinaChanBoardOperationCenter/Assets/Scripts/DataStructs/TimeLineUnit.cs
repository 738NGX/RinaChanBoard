public readonly struct TimeLineUnit
{
    public int Frame { get; }
    public Face Face { get; }
    public TimeLineUnit(int frame, Face face)
    {
        Frame = frame;
        Face = face;
    }
}