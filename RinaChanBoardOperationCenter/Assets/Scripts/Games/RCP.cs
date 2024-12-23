using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RCP : IGame
{
    private Image rinaActionImg;
    private Image playerActionImg;
    private Text roundsText;
    private Text scoreText;
    private Text GameButtonText;
    private Slider roundsSlider;
    private AudioSource rcpVoice;

    private int rinaScore = 0;
    private int playerScore = 0;
    private int rounds = 2;
    private int currentRound = 0;

    private enum RCPAction { Rock, Scissors, Paper }

    protected override void Start()
    {
        base.Start();

        MainTransform = transform.Find("RCP");

        rinaActionImg = MainTransform.Find("RinaAction").GetComponent<Image>();
        playerActionImg = MainTransform.Find("PlayerAction").GetComponent<Image>();
        roundsText = MainTransform.Find("RoundsText").GetComponent<Text>();
        scoreText = MainTransform.Find("ScoreText").GetComponent<Text>();
        roundsSlider = MainTransform.Find("RoundsSlider").GetComponent<Slider>();
        rcpVoice = MainTransform.Find("RCPVoice").GetComponent<AudioSource>();
        GameButtonText = MainTransform.Find("GameButton/Text").GetComponent<Text>();
    }

    public override void StartGame()
    {
        GameButtonText.text = "结束本次猜拳";
        rcpVoice.clip = Resources.Load<AudioClip>("Voice/voice_rcp_1");
        rcpVoice.Play();
        base.StartGame();
        udpServer.LEDs.ClearFace();
        udpServer.LEDs.UpdateFace(new(101, 201, 313, 400));
        udpServer.UploadFaceLite(new(101, 201, 313, 400));
        rinaActionImg.sprite = Resources.Load<Sprite>("Image/Icons/icon_rcp");
        playerActionImg.sprite = Resources.Load<Sprite>("Image/Icons/icon_rcp");
        rinaScore = 0;
        playerScore = 0;
        currentRound = 0;
        scoreText.text = GetScoreText();
    }

    public override void EndGame()
    {
        base.EndGame();
        GameButtonText.text = "开始一次猜拳";
    }

    public void OnGameButtonClick()
    {
        if (isPlaying) return;
        if (inGaming) { EndGame(); }
        else { StartGame(); }
    }
    public void OnChangeRounds()
    {
        if (inGaming)
        {
            roundsSlider.value = rounds;
            return;
        }
        rounds = (int)roundsSlider.value;
        roundsText.text = GetRoundsText();
    }
    public void OnRockClick()
    {
        Action(RCPAction.Rock);
    }
    public void OnScissorsClick()
    {
        Action(RCPAction.Scissors);
    }
    public void OnPaperClick()
    {
        Action(RCPAction.Paper);
    }

    private void Action(RCPAction action)
    {
        if (!inGaming) return;
        StartCoroutine(WaitRinaAction(action));
    }
    private IEnumerator<WaitForSeconds> WaitRinaAction(RCPAction action)
    {
        isPlaying = true;
        udpServer.LEDs.UpdateFace(new(101, 201, 311, 400));
        udpServer.UploadFaceLite(new(101, 201, 311, 400));

        if (currentRound == 0)
        {
            rcpVoice.clip = Resources.Load<AudioClip>("Voice/voice_rcp_2");
            rcpVoice.Play();
            yield return new WaitForSeconds(1.2f);
        }
        else
        {
            rcpVoice.clip = Resources.Load<AudioClip>("Voice/voice_rcp_3");
            rcpVoice.Play();
            yield return new WaitForSeconds(1.1f);
        }

        Dictionary<RCPAction, string> dict_str = new()
        {
            {RCPAction.Rock, "r"},
            {RCPAction.Scissors, "c"},
            {RCPAction.Paper, "p"}
        };
        Dictionary<RCPAction, Face> dict_face = new()
        {
            {RCPAction.Rock, new(101,201,330,402)},
            {RCPAction.Scissors, new(106,201,304,402)},
            {RCPAction.Paper, new(106,206,310,402)}
        };

        RCPAction rinaAction = (RCPAction)Random.Range(0, 3);
        rinaScore += GetScore(rinaAction, action);
        playerScore += GetScore(action, rinaAction);
        scoreText.text = GetScoreText();

        rinaActionImg.sprite = Resources.Load<Sprite>($"Image/Icons/icon_{dict_str[rinaAction]}");
        playerActionImg.sprite = Resources.Load<Sprite>($"Image/Icons/icon_{dict_str[action]}");

        rcpVoice.clip = Resources.Load<AudioClip>($"Voice/voice_rcp_{dict_str[rinaAction]}");
        rcpVoice.Play();
        udpServer.LEDs.UpdateFace(dict_face[rinaAction]);
        udpServer.UploadFaceLite(dict_face[rinaAction]);

        currentRound++;
        if (rinaScore >= rounds || playerScore >= rounds)
        {
            EndGame();
        }
        isPlaying = false;
    }
    private int GetScore(RCPAction action1, RCPAction action2)
    {
        if (action1 == action2) return 0;
        if (action1 == RCPAction.Rock && action2 == RCPAction.Scissors) return 1;
        if (action1 == RCPAction.Scissors && action2 == RCPAction.Paper) return 1;
        if (action1 == RCPAction.Paper && action2 == RCPAction.Rock) return 1;
        return 0;
    }
    private string GetScoreText()
    {
        return $"{rinaScore}  :  {playerScore}";
    }

    private string GetRoundsText()
    {
        return $"{rounds * 2 - 1}局{rounds}胜";
    }
}
