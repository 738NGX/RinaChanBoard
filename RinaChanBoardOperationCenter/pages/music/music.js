const app=getApp();

const face_func=require('../../utils/face_func.js');
const util=require('../../utils/util.js');

const{music_data}=require('../../utils/music_data.js');

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
}

async function play_face(obj,id)
{
    const using_faces=music_data[id].faces;
    for(let i=0;i<using_faces.length;i++)
    {
        face_func.set_face(obj,using_faces[i].leye,using_faces[i].reye,using_faces[i].mouth,using_faces[i].cheek); 
        await sleep(using_faces[i].during*1000/30.0);
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
        music_singers: music_data.map(music => music.singer),
        music_names: music_data.map(music => music.name),
        music_texts: music_data.map(music => music.text),
        cover_srcs: music_data.map(music => music.cover_src),
        choosing_music: 0,
    },
    onLoad() 
    { 
        this.setData({ cells: app.getGlobalCells() });
        // 添加监听回调
        app.addCellsListener((newCells) => {
            this.setData({ cells: newCells });
        });
    },
    onUnload() {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    bindPickerChange: function(e) 
    {
        this.setData({choosing_music: parseInt(e.detail.value)});
    },
    play_music()
    {
        console.log(this.data.choosing_music);
        var src=music_data.map(music => music.music_src)[this.data.choosing_music];
        play_audio(src);
        /*
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
        */
        play_face(this,this.data.choosing_music);
    },
})