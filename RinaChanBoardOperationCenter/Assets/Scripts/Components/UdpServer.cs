using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Text;
using System.Text.RegularExpressions;

public class UdpServer : MonoBehaviour
{
    public bool IsConnected { get; private set; } = false;
    public Leds LEDs { get; private set; }
    public bool SendLiteFace { get; private set; } = true;
    public bool Mutex { get; set; } = false;      // false即关闭时不允许对LEDs的亮度颜色数据进行修改

    private UdpClient udpClient;
    private IPEndPoint localEndPoint = null;
    private IPEndPoint remoteEndPoint = null;
    private Transform manualConnectWindow;
    private Text connectInfo;
    private Text versionText;
    private Text ipCheckText;
    private readonly int remotePort = 1234;
    private readonly int localPort = 4321;
    private readonly float requestTimeout = 1.0f;

    private enum RequestType { Face = 1001, Color = 1002, Bright = 1003, Version = 1004, Battery = 1005 };
    private enum PackTypeLen { Face = 72, Color = 6, Bright = 2 };

    public void ChangeFaceSendMode()
    {
        SendLiteFace = !SendLiteFace;
    }
    public void StartScanDevices()
    {
        StartCoroutine(ScanDevices());
    }
    public void Disconnect()
    {
        IsConnected = false;
        remoteEndPoint = null;
        connectInfo.text = "已断开与璃奈板的连接";
        versionText.text = "璃奈板固件版本: 未连接";
    }
    public void UploadFace()
    {
        if (!IsConnected) return;
        byte[] faceMessage = LedTool.HexToBytes(LEDs.EncodeFace());
        udpClient.Send(faceMessage, faceMessage.Length, remoteEndPoint);
    }
    public void UploadFace(int from, int to)
    {
        if (!IsConnected) return;
        byte[] faceMessage = LedTool.HexToBytes(LEDs.EncodeFace(from, to));
        udpClient.Send(faceMessage, faceMessage.Length, remoteEndPoint);
    }
    public void UploadFaceLite(Face face)
    {
        if (!IsConnected) return;
        byte[] faceMessage = LedTool.HexToBytes(face.LiteFace);
        udpClient.Send(faceMessage, faceMessage.Length, remoteEndPoint);
    }
    public void DownloadFace()
    {
        if (!IsConnected) return;
        StartCoroutine(HandleRequest(RequestType.Face));
    }
    public void UploadBrightness()
    {
        if (!IsConnected) return;
        byte[] brightMessage = LedTool.HexToBytes($"{LEDs.Brightness:X2}");
        udpClient.Send(brightMessage, brightMessage.Length, remoteEndPoint);
    }
    public void DownloadBrightness()
    {
        if (!IsConnected) return;
        StartCoroutine(HandleRequest(RequestType.Bright));
    }
    public void UploadColor()
    {
        if (!IsConnected) return;
        byte[] connectMessage = LedTool.HexToBytes(LedTool.ColorToHex(LEDs.LedOnColor));
        udpClient.Send(connectMessage, connectMessage.Length, remoteEndPoint);
    }
    public void DownloadColor()
    {
        if (!IsConnected) return;
        StartCoroutine(HandleRequest(RequestType.Color));
    }
    public void OnIpAddrInput()
    {
        string ipAddrStr = manualConnectWindow.Find("IPInputField").GetComponent<TMP_InputField>().text;
        if (string.IsNullOrEmpty(ipAddrStr)) return;
        Regex regex = new(@"^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])$");

        if (regex.IsMatch(ipAddrStr) && IPAddress.TryParse(ipAddrStr, out IPAddress ipAddress))
        {
            ipCheckText.text = "目标IP: " + ipAddress.ToString();
            remoteEndPoint = new IPEndPoint(ipAddress, remotePort);
        }
        else
        {
            ipCheckText.text = "请输入正确的IP地址！";
            remoteEndPoint = null;
        }
    }
    public void OnManualConnect()
    {
        if (remoteEndPoint == null)
        {
            return;
        }
        StartCoroutine(TryManualConnect());
    }
    public void OnForceManualConnect()
    {
        remoteEndPoint ??= new IPEndPoint(IPAddress.Broadcast, remotePort);
        StartCoroutine(TryForceManualConnect());
    }
    public void OnManualDisconnect()
    {
        manualConnectWindow.gameObject.SetActive(false);
    }

    private void Start()
    {
        var canvas = GameObject.Find("Canvas").transform;

        LEDs = GameObject.Find("leds").GetComponent<Leds>();
        connectInfo = canvas.Find("Menu/Index/ConnectInfo").GetComponent<Text>();
        versionText = canvas.Find("Menu/Index/VersionText").GetComponent<Text>();
        manualConnectWindow = canvas.Find("ManualConnect");
        ipCheckText = manualConnectWindow.Find("IPCheckText").GetComponent<Text>();

        udpClient = new UdpClient(localPort);
        localEndPoint = new IPEndPoint(IPAddress.Broadcast, remotePort);
        udpClient.EnableBroadcast = true;
    }
    private IEnumerator<WaitForSeconds> ScanDevices()
    {
        byte[] message = LedTool.HexToBytes("1004");
        udpClient.Send(message, message.Length, localEndPoint);

        float startTime = Time.time;
        bool deviceFound = false;

        while (Time.time - startTime < requestTimeout)
        {
            if (udpClient.Available > 0)
            {
                IPEndPoint foundRemoteEndPoint = null;
                byte[] response = udpClient.Receive(ref foundRemoteEndPoint);
                string responseText = Encoding.UTF8.GetString(response);
                Regex regex = new(@"^\d+\.\d+\.\d+-[a-zA-Z]+$");

                if (regex.IsMatch(responseText))
                {
                    connectInfo.text = "已连接到位于" + foundRemoteEndPoint.Address + "的璃奈板";
                    versionText.text = "璃奈板固件版本: " + responseText;
                    remoteEndPoint = foundRemoteEndPoint;
                    deviceFound = true;
                    IsConnected = true;
                    break;
                }
            }
            yield return new WaitForSeconds(0.1f);
        }

        if (deviceFound)
        {
            DownloadBrightness();
            yield return new WaitForSeconds(0.1f);
            DownloadColor();
            yield return new WaitForSeconds(0.1f);
            DownloadFace();
        }
        else
        {
            remoteEndPoint = null;
            IsConnected = false;
            connectInfo.text = "连接失败,未找到璃奈板";

            manualConnectWindow.gameObject.SetActive(true);
        }
    }
    private IEnumerator<WaitForSeconds> TryManualConnect()
    {
        byte[] message = LedTool.HexToBytes("1004");
        udpClient.Send(message, message.Length, remoteEndPoint);

        float startTime = Time.time;
        bool deviceFound = false;

        while (Time.time - startTime < requestTimeout)
        {
            if (udpClient.Available > 0)
            {
                byte[] response = udpClient.Receive(ref remoteEndPoint);
                string responseText = Encoding.ASCII.GetString(response);
                Regex regex = new(@"^\d+\.\d+\.\d+-[a-zA-Z]+$");
        
                if (regex.IsMatch(responseText))
                {
                    connectInfo.text = "已连接到位于" + remoteEndPoint.Address + "的璃奈板";
                    versionText.text = "璃奈板固件版本: " + responseText;
                    deviceFound = true;
                    IsConnected = true;
                    break;
                }
            }
            yield return new WaitForSeconds(0.1f);
        }

        if (deviceFound)
        {
            DownloadBrightness();
            yield return new WaitForSeconds(0.1f);
            DownloadColor();
            yield return new WaitForSeconds(0.1f);
            DownloadFace();
        }
        else
        {
            remoteEndPoint = null;
            IsConnected = false;
            connectInfo.text = "手动连接失败,未找到璃奈板";
        }
        manualConnectWindow.gameObject.SetActive(false);
    }
    private IEnumerator<WaitForSeconds> TryForceManualConnect()
    {
        IsConnected = true;
        DownloadBrightness();
        yield return new WaitForSeconds(0.1f);
        DownloadColor();
        yield return new WaitForSeconds(0.1f);
        DownloadFace();
        connectInfo.text = "已连接到位于" + remoteEndPoint.Address + "的璃奈板";
        versionText.text = "璃奈板固件版本: 未知";
        manualConnectWindow.gameObject.SetActive(false);
    }
    private IEnumerator<WaitForSeconds> HandleRequest(RequestType request)
    {
        byte[] requestMessage = LedTool.HexToBytes(((int)request).ToString());
        udpClient.Send(requestMessage, requestMessage.Length, remoteEndPoint);

        float startTime = Time.time;

        while (Time.time - startTime < requestTimeout)
        {
            if (udpClient.Available > 0)
            {
                byte[] response = udpClient.Receive(ref remoteEndPoint);
                string responseText = LedTool.BytesToHex(response);

                if (request == RequestType.Face && responseText.Length == (int)PackTypeLen.Face)
                {
                    LEDs.UpdateLedsByString(responseText);
                    break;
                }
                else if (request == RequestType.Color && responseText.Length == (int)PackTypeLen.Color)
                {
                    LEDs.UpdateColor(LedTool.HexToColor(responseText));
                    LEDs.WaitColorUpdate = true;
                    break;
                }
                else if (request == RequestType.Bright && responseText.Length == (int)PackTypeLen.Bright)
                {
                    LEDs.Brightness = Convert.ToInt32(responseText, 16);
                    Mutex = true;
                    break;
                }
            }
            yield return new WaitForSeconds(0.1f);
        }
    }
    private void OnApplicationQuit()
    {
        udpClient.Close();
    }
}
