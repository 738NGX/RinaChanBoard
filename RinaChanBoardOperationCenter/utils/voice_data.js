const FPS=30

const voice_data=[
    {
        id:'vo_na_m0209_0001',
        text:'欢迎回来! 璃奈板,笑一个~',
        faces:[
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  6},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during:  1},
            {leye: 2,reye: 2,mouth: 6,cheek: 0,during:  2},
            {leye: 2,reye: 2,mouth: 3,cheek: 0,during:  9},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  7},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  1},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  2},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  1},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  2},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  3},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
        ]
    },
    {
        id:'vo_na_m0209_0002',
        text:'我是......称职的学院偶像吗?',
        faces:[
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  7},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  2},
            {leye: 6,reye: 6,mouth: 6,cheek: 0,during:  3},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  3},
            {leye: 6,reye: 6,mouth: 6,cheek: 0,during:  2},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  2},
            {leye: 6,reye: 6,mouth: 6,cheek: 0,during:  5},
            {leye: 6,reye: 6,mouth: 3,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
        ]
    },
    {
        id:'vo_na_m0209_0003',
        text:'一摸到电脑,就会觉得安心很多~',
        faces:[
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 4,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 1,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  1},
            {leye: 4,reye: 4,mouth: 3,cheek: 0,during:  1},
            {leye: 4,reye: 4,mouth: 6,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
        ]
    },
    {
        id:'vo_na_m0209_0004',
        text:'感觉最近和你亲近了不少......真开心~',
        faces:[
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  8},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  2},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  4},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  1},
            {leye: 1,reye: 1,mouth: 6,cheek: 0,during:  3},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  6},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  2},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  1},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  2},
            {leye: 4,reye: 4,mouth: 3,cheek: 2,during:  6},
            {leye: 4,reye: 4,mouth: 6,cheek: 2,during:  6},
            {leye: 1,reye: 1,mouth: 3,cheek: 0,during:  3},
        ]
    },
]

module.exports={
    voice_data
}