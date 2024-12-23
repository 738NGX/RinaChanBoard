using System.Collections.Generic;
using UnityEngine;

public enum MoreSubPage { Main, Text, Game, LipSync }

public class MorePage : IPage
{
    // 组件
    private MainScene mainScene;        // 主场景

    // 状态
    private bool mutex = false;         // 换页互斥锁
    private List<Transform> subPages;   // 子页面
    
    private MoreSubPage currentSubPage = MoreSubPage.Main;

    private void Start()
    {
        MainTransform = transform.Find("More");

        mainScene = GameObject.Find("Canvas").GetComponent<MainScene>();
        
        subPages = new(){
            MainTransform.Find("Main"), MainTransform.Find("Text"),
            MainTransform.Find("Game"), MainTransform.Find("LipSync")
        };
    }

    public void ChangePageMutex(bool value)
    {
        mutex = value;
    }
    public void OnChangeSubPage(int index)
    {
        ChangeMoreSubPage((MoreSubPage)index);
    }

    private void ChangeMoreSubPage(MoreSubPage page)
    {
        if (mutex || currentSubPage == page) return;

        if (page != MoreSubPage.Main)
            mainScene.ChangePageMutex(true);
        else
            mainScene.ChangePageMutex(false);

        subPages[(int)currentSubPage].gameObject.SetActive(false);
        subPages[(int)page].gameObject.SetActive(true);

        currentSubPage = page;
    }
}
