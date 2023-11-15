const app=getApp();

const face_func=require('../../utils/face_func.js');
const util=require('../../utils/util.js');
const audio = wx.createInnerAudioContext();

const{music_data}=require('../../utils/music_data.js');

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
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

        playing:false,

        current_frame:0,
        current_face:0,
        frames:1,
    },
    onLoad() 
    { 
        this.setData({ cells: app.getGlobalCells() });
        // 添加监听回调
        app.addCellsListener((newCells) => {
            this.setData({ cells: newCells });
        });
        audio.autoplay=false;
    },
    onUnload() {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    bindPickerChange: function(e) 
    {
        let idx=parseInt(e.detail.value);
        const using_faces=music_data[idx].faces;
        this.setData({
            choosing_music: idx,
            frames:using_faces[using_faces.length-1].frame-1,
            current_frame:0,
        });
        audio.src=music_data.map(music=>music.music_src)[this.data.choosing_music];
        face_func.set_face(this,using_faces[0].leye,using_faces[0].reye,using_faces[0].mouth,using_faces[0].cheek); 
    },
    sliderChange: function(e)
    {
        f=parseInt(e.detail.value);
        this.setData({current_frame:f});
        const using_faces=music_data[this.data.choosing_music].faces;
        audio.seek(this.data.current_frame/10);
        for(let i=0;i<using_faces.length;i++)
        {
            if(this.data.current_frame<music_data[this.data.choosing_music].faces[i].frame)
            {
                this.setData({current_face:i-1});
                face_func.set_face(this,using_faces[i-1].leye,using_faces[i-1].reye,using_faces[i-1].mouth,using_faces[i-1].cheek);
                break;
            }
        }
    },
    async play_music()
    {
        this.setData({playing:true});
        const using_faces=music_data[this.data.choosing_music].faces;
        audio.play();
        while(this.data.playing&&this.data.current_frame<this.data.frames)
        {
            for(let i=0;i<using_faces.length;i++)
            {
                if(this.data.current_frame<music_data[this.data.choosing_music].faces[i].frame)
                {
                    if(i-1==this.data.current_face) break;
                    this.setData({current_face:i-1});
                    face_func.set_face(this,using_faces[i-1].leye,using_faces[i-1].reye,using_faces[i-1].mouth,using_faces[i-1].cheek);
                    break;
                }
            }
            var next_frame=(Math.abs(Math.floor(audio.currentTime*10)-this.data.current_frame)>3)?Math.floor(audio.currentTime*10):this.data.current_frame+1;
            this.setData({current_frame:next_frame});
            await sleep(100);
        }
        this.setData({playing:false});
    },
    async stop_music()
    {
        this.setData({playing:false});
        audio.pause();
    }
})