const versions=
[
    {id:'0.0.1',text:' : 第一个测试版本,包含了脱机的自定义表情前端交互.'},
    {id:'0.0.2',text:' : 使用TCP协议完成了和下位机的通讯功能,可以正常完成消息收发.'},
    {id:'0.0.3',text:' : 首页现在可以显示下位机的连接情况;自定义表情现在已经可以控制脸颊.'},
    {id:'0.0.4',text:' : 增加了脱机的预设语音前端交互,虽然现在只包含了一条来自LLAS的测试语音.'},
    {id:'0.0.5',text:' : 现在已经可以在首页上选择不同的控制机型(也可以不选择).'},
    {id:'0.0.6',text:' : 预设语音的播放消息现在已经可以同步到下位机.'},
    {id:'0.0.7',text:' : 增加了脱机的预设歌曲前端交互,包含了测试歌曲Love U my friends(short ver.).'},
    {id:'0.0.8',text:' : 预设歌曲的播放消息现在已经可以同步到下位机.'},
    {id:'0.0.9',text:' : 增加了两条预设语音.'},
    {id:'0.1.0',text:' : 第一个功能基本完备的大版本更新,包括了连接控制/自定义表情/预设语音播放/预设歌曲播放的功能.'},
    {id:'0.1.1',text:' : 修改了自定义表情消息的发送逻辑,现在统一由一个"上传表情"按钮来触发消息发送事件;增加了从璃奈板读取当前显示表情的功能.'},
];

Page({
    data:
    {
        versions:versions,
        current_version:versions[versions.length-1].id,
    },
})