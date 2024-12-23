using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public static class LedTool
{
    public static readonly List<List<int>> ledMap = new()
    {
        new(){ -1, -1, 38, 39, 70, 71,102,103,134,135,166,167,198,199,230,231, -1, -1},
        new(){ -1, 10, 37, 40, 69, 72,101,104,133,136,165,168,197,200,229,232,259, -1},
        new(){  9, 11, 36, 41, 68, 73,100,105,132,137,164,169,196,201,228,233,258,260},
        new(){  8, 12, 35, 42, 67, 74, 99,106,131,138,163,170,195,202,227,234,257,261},
        new(){  7, 13, 34, 43, 66, 75, 98,107,130,139,162,171,194,203,226,235,256,262},
        new(){  6, 14, 33, 44, 65, 76, 97,108,129,140,161,172,193,204,225,236,255,263},
        new(){  5, 15, 32, 45, 64, 77, 96,109,128,141,160,173,192,205,224,237,254,264},
        new(){  4, 16, 31, 46, 63, 78, 95,110,127,142,159,174,191,206,223,238,253,265},
        new(){  3, 17, 30, 47, 62, 79, 94,111,126,143,158,175,190,207,222,239,252,266},
        new(){  2, 18, 29, 48, 61, 80, 93,112,125,144,157,176,189,208,221,240,251,267},
        new(){  1, 19, 28, 49, 60, 81, 92,113,124,145,156,177,188,209,220,241,250,268},
        new(){  0, 20, 27, 50, 59, 82, 91,114,123,146,155,178,187,210,219,242,249,269},
        new(){ -1, 21, 26, 51, 58, 83, 90,115,122,147,154,179,186,211,218,243,248, -1},
        new(){ -1, 22, 25, 52, 57, 84, 89,116,121,148,153,180,185,212,217,244,247, -1},
        new(){ -1, 23, 24, 53, 56, 85, 88,117,120,149,152,181,184,213,216,245,246, -1},
        new(){ -1, -1, -1, 54, 55, 86, 87,118,119,150,151,182,183,214,215, -1, -1, -1},
    };
    public static readonly List<List<string>> colorName = new()
    {
        new(){},
        new(){"穗乃果","绘里","小鸟","海未","凛","真姬","希","花阳","妮可"},
        new(){"千歌","梨子","果南","黛雅","曜","善子","花丸","鞠莉","露比","CYR","AAA","GK","YYY","圣良","理亚","圣雪"},
        new(){"步梦","霞","雫","果林","爱","彼方","雪菜","艾玛","璃奈","栞子","米娅","岚珠","DD","AZN","Q4","R3"},
        new(){"香音","可可","千砂都","堇","恋","希奈子","芽衣","四季","夏美","薇恩","冬毬","猫组","K组","5组"},
        new(){"花帆","沙耶香","梢","缀理","瑠璃乃","慈","吟子","小铃","姬芽","CB","DC","MCP"}
    };
    public static readonly List<List<string>> colorValue = new()
    {
        new(){},
        new(){"f38500","7aeeff","cebfbf","1769ff","fff832","ff503e","c455f6","6ae673","ff4f91"},
        new(){"ff9547","ff9eac","27c1b7","db0839","66c0ff","c1cad4","ffd010","c252c6","ff6fbe","ffa434","ff5a79","825deb","53ab7f","00ccff","bbbbbb","cb3935"},
        new(){"ed7d95","e7d600","01b7ed","485ec6","ff5800","a664a0","d81c2f","84c36e","9ca5b9","37b484","a9a898","f8c8c4","ab76f7","ff0042","d9db83","424a9d"},
        new(){"ff7f27","a0fff9","ff6e90","74f466","0000a0","fff442","ff3535","b2ffdd","ff51c4","e49dfd","4cd2e2","e8243c","bcbcde","ffe840"},
        new(){"f8b500","5383c3","68be8d","ba2636","e7609e","c8c2c6","a2d7dd","fad764","9d8de2","da645f","163bca","F3B171"}
    };
    public static readonly Dictionary<int, FaceModule> faceModules = DatabaseTool.BuildDatabase<FaceModule>("Database/FaceModuleDb");
    public static readonly Dictionary<string, Ascii> asciis = DatabaseTool.BuildAsciiDatabase();
    public static string HexToBinary(string hex)
    {
        StringBuilder binaryString = new();

        foreach (char hexChar in hex)
        {
            int value = int.Parse(hexChar.ToString(), System.Globalization.NumberStyles.HexNumber);
            binaryString.Append(Convert.ToString(value, 2).PadLeft(4, '0'));
        }

        return binaryString.ToString();
    }
    public static string BinaryToHex(string binary)
    {
        StringBuilder hexString = new();

        for (int i = 0; i < binary.Length; i += 4)
        {
            string binaryChunk = binary.Substring(i, 4);
            int value = Convert.ToInt32(binaryChunk, 2);
            hexString.Append(value.ToString("X"));
        }

        return hexString.ToString();
    }
    public static Color HexToColor(string hex)
    {
        if (string.IsNullOrEmpty(hex) || hex.Length != 6)
        {
            Debug.LogError("Invalid hex color string. It should be a 6-character string.");
            return Color.white;
        }

        try
        {
            float r = int.Parse(hex[..2], System.Globalization.NumberStyles.HexNumber) / 255f;
            float g = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
            float b = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;

            return new Color(r, g, b);
        }
        catch
        {
            Debug.LogError("Failed to parse hex color string.");
            return Color.white;
        }
    }
    public static string ColorToHex(Color color)
    {
        int r = Mathf.RoundToInt(color.r * 255);
        int g = Mathf.RoundToInt(color.g * 255);
        int b = Mathf.RoundToInt(color.b * 255);

        return $"{r:X2}{g:X2}{b:X2}".ToLower();
    }
    public static byte[] HexToBytes(string hex)
    {
        if (string.IsNullOrEmpty(hex))
            Debug.LogError("空字符串");

        if (hex.Length % 2 != 0)
            Debug.LogError("字符串长度非偶数");

        byte[] bytes = new byte[hex.Length / 2];
        for (int i = 0; i < bytes.Length; i++)
        {
            bytes[i] = Convert.ToByte(hex.Substring(i * 2, 2), 16);
        }
        return bytes;
    }
    public static string BytesToHex(byte[] bytes)
    {
        if (bytes == null || bytes.Length == 0)
            Debug.LogError("空数组");

        StringBuilder hexString = new(bytes.Length * 2);
        foreach (byte b in bytes)
        {
            hexString.Append(b.ToString("x2"));
        }
        return hexString.ToString();
    }
}