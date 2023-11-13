const device_info=[
    {uid:'null',topic:'null'},
    {uid:'a8a83e1f0a4c4e42b031e1c323dd9159',topic:'RinaChanBoard'},
    {uid:'a8a83e1f0a4c4e42b031e1c323dd9159',topic:'RinaChanBoardExp'},
];

const color_info=[
    {name:'默认-璃奈粉色',color:'f971d4'},
    {name:'高坂穗乃果-橙色',color:'f38500'},
    {name:'绚濑绘里-水蓝色',color:'7aeeff'},
    {name:'南小鸟-白色',color:'cebfbf'},
    {name:'园田海未-蓝色',color:'1769ff'},
    {name:'星空凛-黄色',color:'fff832'},
    {name:'西木野真姬-红色',color:'ff503e'},
    {name:'东条希-紫罗兰色',color:'c455f6'},
    {name:'小泉花阳-绿色',color:'6ae673'},
    {name:'矢泽妮可-粉色',color:'ff4f91'},
    {name:'高海千歌-蜜柑色',color:'ff9547'},
    {name:'樱内梨子-樱花粉色',color:'ff9eac'},
    {name:'松浦果南-祖母绿色',color:'27c1b7'},
    {name:'黑泽黛雅-红色',color:'db0839'},
    {name:'渡边曜-亮蓝色',color:'66c0ff'},
    {name:'津岛善子-白色',color:'c1cad4'},
    {name:'国木田花丸-黄色',color:'ffd010'},
    {name:'小原鞠莉-紫罗兰色',color:'c252c6'},
    {name:'黑泽露比-粉色',color:'ff6fbe'},
    {name:'CYaRon!-橙色',color:'ffa434'},
    {name:'AZALEA-粉色',color:'ff5a79'},
    {name:'Guilty Kiss-紫色',color:'825deb'},
    {name:'鹿角圣良-天蓝色',color:'00ccff'},
    {name:'鹿角理亚-纯白色',color:'bbbbbb'},
    {name:'Saint Snow-红色',color:'cb3935'},
    {name:'高咲侑-黑色',color:'1d1d1d'},
    {name:'上原步梦-浅粉色',color:'ed7d95'},
    {name:'中须霞-蜡笔黄色',color:'e7d600'},
    {name:'樱坂雫-浅蓝色',color:'01b7ed'},
    {name:'朝香果林-皇室蓝色',color:'485ec6'},
    {name:'宫下爱-超橙色',color:'ff5800'},
    {name:'近江彼方-堇色',color:'a664a0'},
    {name:'优木雪菜-猩红色',color:'d81c2f'},
    {name:'艾玛·维尔德-浅绿色',color:'84c36e'},
    {name:'天王寺璃奈-纸白色',color:'9ca5b9'},
    {name:'三船栞子-翡翠色',color:'37b484'},
    {name:'米雅·泰勒-白金银色',color:'a9a898'},
    {name:'钟岚珠-玫瑰金色',color:'f8c8c4'},
    {name:'DiverDiva-银紫色',color:'ab76f7'},
    {name:'A·ZU·NA-意大利红色',color:'ff0042'},
    {name:'QU4RTZ-奶茶色',color:'d9db83'},
    {name:'R3BIRTH-坦桑蓝色',color:'424a9d'},
    {name:'涩谷香音-金盏花色',color:'ff7f27'},
    {name:'唐可可-蜡笔蓝色',color:'a0fff9'},
    {name:'岚千砂都-桃粉色',color:'ff6e90'},
    {name:'平安名堇-蜜瓜绿色',color:'74f466'},
    {name:'叶月恋-宝石蓝色',color:'0000a0'},
    {name:'樱小路希奈子-玉米黄色',color:'fff442'},
    {name:'米女芽衣-胭脂红色',color:'ff3535'},
    {name:'若菜四季-冰绿白色',color:'b2ffdd'},
    {name:'鬼冢夏美-鬼夏粉色',color:'ff51c4'},
    {name:'薇恩·玛格丽特-优雅紫色',color:'e49dfd'},
    {name:'鬼冢冬毬-烟熏蓝色',color:'4cd2e2'},
];

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
}

module.exports = 
{
    device_info,sleep,color_info,
};