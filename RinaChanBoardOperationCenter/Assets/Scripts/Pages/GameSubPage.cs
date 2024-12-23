using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum GameType { Main, RCP }

public class GameSubPage : IPage
{
    private List<Transform> gamePages;   // 子页面
    
    private GameType currentGame = GameType.Main;
    private bool isGaming = false;

    private void Start()
    {
        MainTransform = transform.Find("Game");
        gamePages = new(){
            MainTransform.Find("Main"), MainTransform.Find("RCP")
        };
    }
    
    public void OnChangeGameClick(int index)
    {
        ChangeGame((GameType)index);
    }

    public void SetGameStatus(bool status)
    {
        isGaming = status;
    }
    
    private void ChangeGame(GameType game)
    {
        if (isGaming || currentGame == game) return;

        gamePages[(int)currentGame].gameObject.SetActive(false);
        gamePages[(int)game].gameObject.SetActive(true);

        currentGame = game;
    }
}
