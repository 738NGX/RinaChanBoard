const device_info=[
    {uid:'null',topic:'null'},
    {uid:'a8a83e1f0a4c4e42b031e1c323dd9159',topic:'RinaChanBoard'},
    {uid:'a8a83e1f0a4c4e42b031e1c323dd9159',topic:'RinaChanBoardExp'},
]

function sleep (time) 
{
    return new Promise((resolve) => setTimeout(resolve, time));
}

module.exports = 
{
    device_info,sleep
};