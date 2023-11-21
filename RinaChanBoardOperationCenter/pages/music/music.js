const app=getApp();

const face_func=require('../../utils/face_func.js');
const util=require('../../utils/util.js');

const{music_data}=require('../../utils/music_data.js');

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
}

function frame2time(frame)
{
    let min=Math.floor(frame/600).toString();
    if(min.length==1) min='0'+min;
    let sec=Math.floor(frame%600/10).toString();
    if(sec.length==1) sec='0'+sec;
    return min+':'+sec+'.'+(frame%10).toString();
}

Page({
    data: 
    {
        cells: [],
        music_data:music_data,
        music_singers: music_data.map(music => music.singer),
        music_names: music_data.map(music => music.name),
        music_texts: music_data.map(music => music.text),
        cover_srcs: music_data.map(music => music.cover_src),
        choosing_music: 0,
        music_src:'',

        playing:false,

        current_frame:0,
        current_time:'00:00.0',
        current_face:0,
        frames:1,
        during:'00:00.1',
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
        let idx=parseInt(e.detail.value);
        const using_faces=music_data[idx].faces;
        this.setData({
            choosing_music: idx,
            frames:using_faces[using_faces.length-1].frame-1,
            current_frame:0,
            current_face:0,
            current_time:'00:00.0',
            during:frame2time(using_faces[using_faces.length-1].frame-1),
        });
        this.setData({music_src:music_data.map(music=>music.music_src)[this.data.choosing_music]});
        face_func.set_face(this,using_faces[0].leye,using_faces[0].reye,using_faces[0].mouth,using_faces[0].cheek); 
    },
    sliderChange: function(e)
    {
        this.setData({playing:false});
        f=parseInt(e.detail.value);
        this.setData({current_frame:f,currentTime:frame2time(f)});
        const using_faces=music_data[this.data.choosing_music].faces;
        
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
        if(this.data.playing) return;
        const using_faces=music_data[this.data.choosing_music].faces;
        
        let start=this.data.current_frame.toString();
        if(start.length==1) start='000'+start;
        else if(start.length==2) start='00'+start;
        else if(start.length==3) start='0'+start;
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: util.device_info[app.get_controlling_device()].uid,
                topic: util.device_info[app.get_controlling_device()].topic,
                msg: music_data[this.data.choosing_music].id+start
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data);
            }
        });
        
        const audio=wx.createInnerAudioContext();
        audio.autoplay=false;
        audio.src=this.data.music_src;
        if(this.data.current_frame>=this.data.frames)
        {
            audio.seek(0);
            this.setData({current_frame:0,current_time:'00:00.0'});
        }
        else
        {
            audio.seek(Math.floor(this.data.current_frame/10));
        }
        face_func.update_face_to_server(this.data.cells);
        this.setData({playing:true});
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
                    face_func.update_face_to_server(this.data.cells);
                    break;
                }
            }
            var next_frame=(Math.abs(Math.floor(audio.currentTime*10)-this.data.current_frame)>3)?Math.floor(audio.currentTime*10):this.data.current_frame+1;
            this.setData({current_frame:next_frame,current_time:frame2time(next_frame)});
            await sleep(100);
        }
        
        audio.pause();
        this.setData({playing:false});
    },
    async stop_music()
    {
        this.setData({playing:false});
    }
})