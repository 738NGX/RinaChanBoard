/**
 * 歌曲id的命名规则如下:
 * music_团名_歌手名_歌曲编号_站位_完整版/短版
 * 
 * 团名: 00缪 01水 02虹 03星
 * 
 * 歌手名: 
 * 00 团曲 
 * 01 穗乃果 千歌 步梦 香音
 * 02 绘里 梨子 霞 可可
 * 03 小鸟 果南 雫 千砂都
 * 04 海未 黛雅 果林 堇
 * 05 凛 曜 爱 恋
 * 06 真姬 善子 彼方 希奈子
 * 07 希 花丸 雪菜 芽衣
 * 08 花阳 鞠莉 艾玛 四季
 * 09 妮可 露比 璃奈 夏美
 * 10 / / 栞子 薇恩
 * 11 / / 米娅 冬毬
 * 12 / / 岚珠 /
 * 21 Printemps CYaRon AZuNa CatChu
 * 22 BiBi AZALEA QU4RTZ KALEIDOSCORE
 * 23 LilyWhite GuiltyKiss DiverDiva 5yncri5e
 * 24 / / R3Birth /
 * 31 A-Rise SaintSnow / SunnyPassion
 * 
 * 歌曲编号按照前置条件下的发售顺序
 * 站位按照原歌曲对应歌手编号的站位
 * 完整版为1,短版/为0
 */

const music_data=[
    {id:'none',name:'未选择',singer:'未选择',text:'选一首你想要播放的歌曲吧!'},
    {
        id:'music_02_00_02_09_0',
        name:'Love U my friends(Short ver.)',
        cover_src:'https://www.738ngx.site/api/rinachanboard/images/cover/music_02_00_02.png',
        music_src:'https://www.738ngx.site/api/rinachanboard/musics/music_02_00_02_0.mp3',
        singer:'虹ヶ咲学園スクールアイドル同好会',
        text:  '虹咲学园学园偶像同好会第二张专辑《Love U my friends》的同名曲。',
        faces:[
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:200},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:160},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 31},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 44},
            {leye: 6,reye: 6,mouth: 5,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 54},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during: 21},
            {leye: 1,reye: 4,mouth: 3,cheek: 2,during:  9},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 10},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 12},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 21},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 36},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 29},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 55},
            {leye: 2,reye: 2,mouth: 9,cheek: 0,during:  5},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during:  4},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  8},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during: 18},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  9},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 49},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:183},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 10},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 12},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 21},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 15},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  8},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 14},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:170},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 22},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 30},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 24},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  6},
            {leye: 6,reye: 6,mouth: 5,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 19},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 27},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  9},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during: 12},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 15},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during:  8},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  6},
            {leye: 2,reye: 2,mouth: 9,cheek: 0,during:  6},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  4},
            {leye: 2,reye: 2,mouth: 9,cheek: 0,during:  7},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  2},
            {leye: 2,reye: 2,mouth:10,cheek: 0,during: 11},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 24},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 12},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 15},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 16},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during: 16},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 18},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 11},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 18},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 30},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 25},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 19},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 16},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 18},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 24},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 10},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 21},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 17},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 12},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 15},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during: 20},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  6},
            {leye: 6,reye: 6,mouth: 9,cheek: 0,during: 13},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 22},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during: 23},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 60},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  9},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 15},
            {leye: 6,reye: 6,mouth: 6,cheek: 0,during:  6},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  4},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:  6},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 9,cheek: 0,during:  5},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 65},
            {leye: 1,reye: 1,mouth: 5,cheek: 0,during: 29},
            {leye: 6,reye: 6,mouth: 5,cheek: 0,during: 24},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 18},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 17},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:126},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 12},
            {leye: 1,reye: 4,mouth: 3,cheek: 2,during: 14},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during: 98},
            {leye: 1,reye: 1,mouth:10,cheek: 0,during:135},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during: 52},
        ]
    },
]

module.exports={
    music_data
}