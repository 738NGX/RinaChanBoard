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
    const audio=wx.createInnerAudioContext();
    audio.autoplay=false;
    audio.src='https://www.738ngx.site/api/rinachanboard/voices/'+id+'.mp3';;
    audio.play();

    for(let i=0;i<using_faces.length;i++)
    {
        face_func.set_face(obj,using_faces[i].leye,using_faces[i].reye,using_faces[i].mouth,using_faces[i].cheek); 
        face_func.update_face_to_server(obj.data.cells)
        await sleep(using_faces[i].during*100);
    }
}

Page({
    data: 
    {
        cells: [],
        voices: voice_data,
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
        play_face(this,e.currentTarget.dataset.id)
    },
})