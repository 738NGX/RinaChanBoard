const app=getApp();

const face_func=require('../../utils/face_func.js');
const util=require('../../utils/util.js');

const{voice_data}=require('../../utils/voice_data.js');

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
}

async function play_face(obj,id)
{
    const using_faces=voice_data.find(function (obj){return obj.id ==id;}).faces;
    for(let i=0;i<using_faces.length;i++)
    {
        face_func.set_face(obj,using_faces[i].leye,using_faces[i].reye,using_faces[i].mouth,using_faces[i].cheek); 
        await sleep(using_faces[i].during*33);
    }
}

function play_audio(src)
{
    const audio=wx.createInnerAudioContext();
    audio.autoplay=false;
    audio.src=src;
    console.log('Playing '+src)
    audio.play();
}

Page({
    data: 
    {
        cells: [],
        voices: 
        [
            {id:'vo_na_m0209_0001',text:voice_data[0].text},
        ],
    },
    onLoad() 
    { 
        this.setData({ cells: app.getGlobalCells() });
        // 添加监听回调
        app.addCellsListener((newCells) => {
            this.setData({ cells: newCells });
        });
    },
    onUnload() 
    {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    play_voice(e)
    {
        var src='https://www.738ngx.site/api/rinachanboard/voices/'+e.currentTarget.dataset.id+'.mp3';
        play_audio(src);
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: util.device_info[app.get_controlling_device()].uid,
                topic: util.device_info[app.get_controlling_device()].topic,
                msg: e.currentTarget.dataset.id
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data);
            }
        });
        play_face(this,e.currentTarget.dataset.id)
    },
})