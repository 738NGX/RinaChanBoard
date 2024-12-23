using UnityEngine;

public class IGame : IPage
{
    protected UdpServer udpServer;
    protected bool inGaming = false;
    protected bool isPlaying = false;
    protected bool stopPlay = false;

    protected virtual void Start()
    {
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
    }

    public virtual void StartGame()
    {
        inGaming = true;
        transform.parent.GetComponent<GameSubPage>().SetGameStatus(true);
    }

    public virtual void EndGame()
    {
        transform.parent.GetComponent<GameSubPage>().SetGameStatus(false);
        inGaming = false;
    }
}
