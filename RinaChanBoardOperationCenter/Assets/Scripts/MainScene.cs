using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;
using DG.Tweening;

public enum Page { Index, Custom, Voice, Music, Anime, More, About };

public class MainScene : MonoBehaviour
{
    // 公用组件
    private UdpServer udpServer;        // Udp服务
    
    private Image frame;                // 璃奈板显示底框
    private GameObject display;         // 璃奈板显示
    private Transform menu;             // 菜单
    private Transform menuTabs;         // 菜单页面标签
    private List<GameObject> pages;     // 菜单页面列表
    

    // 公用状态
    private bool pageMutex = false;
    private bool isConnected = false;
    private Page currentPage = Page.Index;

    private void Start()
    {
        udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
        frame = GameObject.Find("frame").GetComponent<Image>();
        display = GameObject.Find("Display");
        menuTabs = GameObject.Find("MenuTabs").transform;

        menu = GameObject.Find("Menu").transform;
        pages = new(){
            menu.Find("Index").gameObject, menu.Find("Custom").gameObject, menu.Find("Voice").gameObject,
            menu.Find("Music").gameObject, menu.Find("Anime").gameObject, menu.Find("More").gameObject,
            menu.Find("About").gameObject
        };

        display.SetActive(false);

        foreach (Transform tab in menuTabs)
        {
            tab.gameObject.GetComponent<Button>().onClick.AddListener(() =>
            {
                ChangePage((Page)tab.GetSiblingIndex());
            });
        }
    }
    private void Update()
    {
        // 监听连接情况,改变时触发回调进行同步
        if (udpServer.IsConnected && !isConnected)
        {
            isConnected = true;
            display.SetActive(true);
            GetComponent<RinaChanDressManager>().ChangeConnectionStatus();
            menu.GetComponent<IndexPage>().SetUdpServerButtonText("断开");
        }
        else if (!udpServer.IsConnected && isConnected)
        {
            isConnected = false;
            display.SetActive(false);
            GetComponent<RinaChanDressManager>().ChangeConnectionStatus();
            menu.GetComponent<IndexPage>().SetUdpServerButtonText("连接");
        }
    }
    // 改变页面互斥锁
    public void ChangePageMutex(bool mutex)
    {
        pageMutex = mutex;
    }
    // 改变显示页面
    public void ChangePage(Page page)
    {
        // 同页面或页面互斥锁启用时无法换页
        if (page == currentPage || pageMutex)
        {
            return;
        }
        // 控制页面在未连接时不可用
        if (!isConnected && page != Page.Index && page != Page.About)
        {
            return;
        }

        // Tab背景与文字颜色变换
        menuTabs.GetChild((int)currentPage).GetComponent<Image>().color = new(1f, 1f, 1f, 0f);
        menuTabs.GetChild((int)page).GetComponent<Image>().color = new(1f, 1f, 1f, 0.8f);
        menuTabs.GetChild((int)currentPage).GetChild(0).GetComponent<Text>().color = new(1f, 1f, 1f, 1f);
        menuTabs.GetChild((int)page).GetChild(0).GetComponent<Text>().color = new(0f, 0f, 0f, 1f);

        pages[(int)currentPage].SetActive(false);
        pages[(int)page].SetActive(true);

        // 切换到或切换出自定表情页面时需要播放切换动画
        if (NeedFrameAnimation(page))
        {
            DoFrameAnimation();
        }
        currentPage = page;
    }
    // 连接按钮回调
    public void OnUdpServerButtonClicked()
    {
        if (isConnected) { udpServer.Disconnect(); }
        else { udpServer.StartScanDevices(); }
    }
    // 判断是否需要播放切换动画
    private bool NeedFrameAnimation(Page page)
    {
        return currentPage == Page.Custom && page != Page.Custom ||
               currentPage != Page.Custom && page == Page.Custom;
    }
    // 执行切换动画
    private void DoFrameAnimation()
    {
        if (currentPage == Page.Custom)
        {
            udpServer.LEDs.CanCustom = false;
            display.transform.DOScale(new Vector3(0.42f, 0.4f, 1f), 0.5f);
            display.transform.DOLocalMoveY(30, 0.5f);
            display.transform.DOLocalRotate(new Vector3(0, 0, 5f), 0.5f);
            GetComponent<RinaChanDressManager>().Rina.DOFade(1f, 0.5f);
            frame.DOFade(1f, 0.5f);
        }
        else
        {
            udpServer.LEDs.CanCustom = true;
            display.transform.DOScale(new Vector3(1f, 1f, 1f), 0.5f);
            display.transform.DOLocalMoveY(0, 0.5f);
            display.transform.DOLocalRotate(new Vector3(0, 0, 0), 0.5f);
            GetComponent<RinaChanDressManager>().Rina.DOFade(0, 0.5f);
            frame.DOFade(0.8f, 0.5f);
        }
    }
}
