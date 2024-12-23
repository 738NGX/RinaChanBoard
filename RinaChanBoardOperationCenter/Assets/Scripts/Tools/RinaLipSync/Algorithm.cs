using Unity.Collections;
using Unity.Mathematics;
using Unity.Burst;
using Unity.Collections.LowLevel.Unsafe;

namespace RinaLipSync
{
    [BurstCompile]
    public static unsafe class Algorithm
    {
        // 计算数组中绝对值的最大值,用于快速找到音频信号的最大振幅
        public static float GetMaxValue(in NativeArray<float> array)
        {
            return GetMaxValue((float*)array.GetUnsafeReadOnlyPtr(), array.Length);
        }

        [BurstCompile]
        static float GetMaxValue(float* array, int len)
        {
            float max = 0f;
            for (int i = 0; i < len; ++i)
            {
                max = math.max(max, math.abs(array[i]));
            }
            return max;
        }

        // 计算音频信号的均方根(RMS)音量,反映信号的总体强度
        public static float GetRMSVolume(in NativeArray<float> array)
        {
            return GetRMSVolume((float*)array.GetUnsafeReadOnlyPtr(), array.Length);
        }

        [BurstCompile]
        static float GetRMSVolume(float* array, int len)
        {
            float average = 0f;
            for (int i = 0; i < len; ++i)
            {
                average += array[i] * array[i];
            }
            return math.sqrt(average / len);
        }

        // 实现循环缓冲区的数据复制功能,用于处理循环队列或滑动窗口
        public static void CopyRingBuffer(in NativeArray<float> input, out NativeArray<float> output, int startSrcIndex)
        {
            output = new NativeArray<float>(input.Length, Allocator.Temp);
            CopyRingBuffer(
                (float*)input.GetUnsafeReadOnlyPtr(),
                (float*)output.GetUnsafePtr(),
                input.Length,
                startSrcIndex);
        }

        [BurstCompile]
        static void CopyRingBuffer(float* input, float* output, int len, int startSrcIndex)
        {
            for (int i = 0; i < len; ++i)
            {
                output[i] = input[(startSrcIndex + i) % len];
            }
        }

        // 将音频信号归一化到特定的范围(默认最大值为1),以标准化数据幅度
        public static void Normalize(ref NativeArray<float> array, float value = 1f)
        {
            Normalize((float*)array.GetUnsafePtr(), array.Length, value);
        }

        [BurstCompile]
        static void Normalize(float* array, int len, float value = 1f)
        {
            float max = GetMaxValue(array, len);
            if (max < math.EPSILON) return;
            float r = value / max;
            for (int i = 0; i < len; ++i)
            {
                array[i] *= r;
            }
        }

        // 实现低通滤波器,用于去除音频信号中高频噪声
        public static void LowPassFilter(ref NativeArray<float> data, float sampleRate, float cutoff, float range)
        {
            cutoff = (cutoff - range) / sampleRate;
            range /= sampleRate;

            var tmp = new NativeArray<float>(data, Allocator.Temp);

            int n = (int)math.round(3.1f / range);
            if ((n + 1) % 2 == 0) n += 1;
            var b = new NativeArray<float>(n, Allocator.Temp);

            LowPassFilter(
                (float*)data.GetUnsafePtr(),
                data.Length,
                cutoff,
                (float*)tmp.GetUnsafeReadOnlyPtr(),
                (float*)b.GetUnsafePtr(),
                n);

            tmp.Dispose();
            b.Dispose();
        }

        [BurstCompile]
        static void LowPassFilter(float* data, int len, float cutoff, float* tmp, float* b, int bLen)
        {
            for (int i = 0; i < bLen; ++i)
            {
                float x = i - (bLen - 1) / 2f;
                float ang = 2f * math.PI * cutoff * x;
                b[i] = 2f * cutoff * math.sin(ang) / ang;
            }

            for (int i = 0; i < len; ++i)
            {
                for (int j = 0; j < bLen; ++j)
                {
                    if (i - j >= 0)
                    {
                        data[i] += b[j] * tmp[i - j];
                    }
                }
            }
        }

        // 对信号进行降采样,将高采样率数据转换为目标采样率
        public static void DownSample(in NativeArray<float> input, out NativeArray<float> output, int sampleRate, int targetSampleRate)
        {
            if (sampleRate <= targetSampleRate)
            {
                output = new NativeArray<float>(input, Allocator.Temp);
            }
            else if (sampleRate % targetSampleRate == 0)
            {
                int skip = sampleRate / targetSampleRate;
                output = new NativeArray<float>(input.Length / skip, Allocator.Temp);
                DownSample1(
                    (float*)input.GetUnsafeReadOnlyPtr(),
                    (float*)output.GetUnsafePtr(),
                    output.Length,
                    skip);
            }
            else
            {
                float df = (float)sampleRate / targetSampleRate;
                int n = (int)math.round(input.Length / df);
                output = new NativeArray<float>(n, Allocator.Temp);
                DownSample2(
                    (float*)input.GetUnsafeReadOnlyPtr(),
                    input.Length,
                    (float*)output.GetUnsafePtr(),
                    output.Length,
                    df);
            }
        }

        [BurstCompile]
        static void DownSample1(float* input, float* output, int outputLen, int skip)
        {
            for (int i = 0; i < outputLen; ++i)
            {
                output[i] = input[i * skip];
            }
        }

        [BurstCompile]
        static void DownSample2(float* input, int inputLen, float* output, int outputLen, float df)
        {
            for (int j = 0; j < outputLen; ++j)
            {
                float fIndex = df * j;
                int i0 = (int)math.floor(fIndex);
                int i1 = math.min(i0, inputLen - 1);
                float t = fIndex - i0;
                float x0 = input[i0];
                float x1 = input[i1];
                output[j] = math.lerp(x0, x1, t);
            }
        }

        // 应用预加重滤波器,提高高频部分的能量,用于语音增强和特征提取
        public static void PreEmphasis(ref NativeArray<float> data, float p)
        {
            var tmp = new NativeArray<float>(data, Allocator.Temp);
            PreEmphasis(
                (float*)data.GetUnsafePtr(),
                (float*)tmp.GetUnsafeReadOnlyPtr(),
                data.Length,
                p);
            tmp.Dispose();
        }

        [BurstCompile]
        static void PreEmphasis(float* data, float* tmp, int len, float p)
        {
            for (int i = 1; i < len; ++i)
            {
                data[i] = tmp[i] - p * tmp[i - 1];
            }
        }

        // 对信号施加汉明窗,用于减少信号分段时的频谱泄漏
        public static void HammingWindow(ref NativeArray<float> array)
        {
            HammingWindow((float*)array.GetUnsafePtr(), array.Length);
        }

        [BurstCompile]
        static void HammingWindow(float* array, int len)
        {
            for (int i = 0; i < len; ++i)
            {
                float x = (float)i / (len - 1);
                array[i] *= 0.54f - 0.46f * math.cos(2f * math.PI * x);
            }
        }

        // 在信号两端添加零,扩展信号长度,用于频谱分析等场景
        public static void ZeroPadding(ref NativeArray<float> data, out NativeArray<float> dataWithPadding)
        {
            int N = data.Length;
            dataWithPadding = new NativeArray<float>(N * 2, Allocator.Temp);

            var slice1 = new NativeSlice<float>(dataWithPadding, 0, N / 2);
            UnsafeUtility.MemSet((float*)slice1.GetUnsafePtr<float>(), 0, sizeof(float) * slice1.Length);

            var slice2 = new NativeSlice<float>(dataWithPadding, N / 2, N);
            slice2.CopyFrom(data);

            var slice3 = new NativeSlice<float>(dataWithPadding, N * 3 / 2, N / 2);
            UnsafeUtility.MemSet((float*)slice3.GetUnsafePtr<float>(), 0, sizeof(float) * slice1.Length);
        }

        // 计算信号的频谱,用于分析信号在频域的分布
        public static void FFT(in NativeArray<float> data, out NativeArray<float> spectrum)
        {
            int N = data.Length;
            spectrum = new NativeArray<float>(N, Allocator.Temp);
            FFT1((float*)data.GetUnsafePtr(), (float*)spectrum.GetUnsafePtr(), N);
        }

        [BurstCompile]
        static void FFT1(float* data, float* spectrum, int N)
        {
            var spectrumRe = new NativeArray<float>(N, Allocator.Temp);
            var spectrumIm = new NativeArray<float>(N, Allocator.Temp);

            for (int i = 0; i < N; ++i)
            {
                spectrumRe[i] = data[i];
            }
            FFT2((float*)spectrumRe.GetUnsafePtr(), (float*)spectrumIm.GetUnsafePtr(), N);

            for (int i = 0; i < N; ++i)
            {
                float re = spectrumRe[i];
                float im = spectrumIm[i];
                spectrum[i] = math.length(new float2(re, im));
            }

            spectrumRe.Dispose();
            spectrumIm.Dispose();
        }

        [BurstCompile]
        static void FFT2(float* spectrumRe, float* spectrumIm, int N)
        {
            if (N < 2) return;

            var evenRe = new NativeArray<float>(N / 2, Allocator.Temp);
            var evenIm = new NativeArray<float>(N / 2, Allocator.Temp);
            var oddRe = new NativeArray<float>(N / 2, Allocator.Temp);
            var oddIm = new NativeArray<float>(N / 2, Allocator.Temp);

            for (int i = 0; i < N / 2; ++i)
            {
                evenRe[i] = spectrumRe[i * 2];
                evenIm[i] = spectrumIm[i * 2];
                oddRe[i] = spectrumRe[i * 2 + 1];
                oddIm[i] = spectrumIm[i * 2 + 1];
            }

            FFT2((float*)evenRe.GetUnsafePtr(), (float*)evenIm.GetUnsafePtr(), N / 2);
            FFT2((float*)oddRe.GetUnsafePtr(), (float*)oddIm.GetUnsafePtr(), N / 2);

            for (int i = 0; i < N / 2; ++i)
            {
                float er = evenRe[i];
                float ei = evenIm[i];
                float or = oddRe[i];
                float oi = oddIm[i];
                float theta = -2f * math.PI * i / N;
                var c = new float2(math.cos(theta), math.sin(theta));
                c = new float2(c.x * or - c.y * oi, c.x * oi + c.y * or);
                spectrumRe[i] = er + c.x;
                spectrumIm[i] = ei + c.y;
                spectrumRe[N / 2 + i] = er - c.x;
                spectrumIm[N / 2 + i] = ei - c.y;
            }

            evenRe.Dispose();
            evenIm.Dispose();
            oddRe.Dispose();
            oddIm.Dispose();
        }

        // 将频谱转换为Mel尺度频谱,用于语音信号的MFCC特征提取
        public static void MelFilterBank(
            in NativeArray<float> spectrum,
            out NativeArray<float> melSpectrum,
            float sampleRate,
            int melDiv)
        {
            melSpectrum = new NativeArray<float>(melDiv, Allocator.Temp);
            MelFilterBank(
                (float*)spectrum.GetUnsafeReadOnlyPtr(),
                (float*)melSpectrum.GetUnsafePtr(),
                spectrum.Length,
                sampleRate,
                melDiv);
        }

        [BurstCompile]
        static void MelFilterBank(
            float* spectrum,
            float* melSpectrum,
            int len,
            float sampleRate,
            int melDiv)
        {
            float fMax = sampleRate / 2;
            float melMax = ToMel(fMax);
            int nMax = len / 2;
            float df = fMax / nMax;
            float dMel = melMax / (melDiv + 1);

            for (int n = 0; n < melDiv; ++n)
            {
                float melBegin = dMel * n;
                float melCenter = dMel * (n + 1);
                float melEnd = dMel * (n + 2);

                float fBegin = ToHz(melBegin);
                float fCenter = ToHz(melCenter);
                float fEnd = ToHz(melEnd);

                int iBegin = (int)math.ceil(fBegin / df);
                int iCenter = (int)math.round(fCenter / df);
                int iEnd = (int)math.floor(fEnd / df);

                float sum = 0f;
                for (int i = iBegin + 1; i <= iEnd; ++i)
                {
                    float f = df * i;
                    float a = (i < iCenter) ?
                        (f - fBegin) / (fCenter - fBegin) :
                        (fEnd - f) / (fEnd - fCenter);
                    a /= (fEnd - fBegin) * 0.5f;
                    sum += a * spectrum[i];
                }
                melSpectrum[n] = sum;
            }
        }

        // 将信号功率转换为分贝(dB)表示,用于更直观的表示信号强度
        public static void PowerToDb(ref NativeArray<float> array)
        {
            PowerToDb((float*)array.GetUnsafePtr(), array.Length);
        }

        [BurstCompile]
        static void PowerToDb(float* array, int len)
        {
            for (int i = 0; i < len; ++i)
            {
                array[i] = 10f * math.log10(array[i]);
            }
        }

        // 实现频率和Mel尺度之间的互相转换,用于Mel滤波器的设计
        [BurstCompile]
        static float ToMel(float hz, bool slaney = false)
        {
            float a = slaney ? 2595f : 1127f;
            return a * math.log(hz / 700f + 1f);
        }

        [BurstCompile]
        static float ToHz(float mel, bool slaney = false)
        {
            float a = slaney ? 2595f : 1127f;
            return 700f * (math.exp(mel / a) - 1f);
        }

        // 实现离散余弦变换,用于从Mel频谱生成MFCC特征
        public static void DCT(
            in NativeArray<float> spectrum,
            out NativeArray<float> cepstrum)
        {
            cepstrum = new NativeArray<float>(spectrum.Length, Allocator.Temp);
            DCT(
                (float*)spectrum.GetUnsafeReadOnlyPtr(),
                (float*)cepstrum.GetUnsafePtr(),
                spectrum.Length);
        }

        [BurstCompile]
        static void DCT(
            float* spectrum,
            float* cepstrum,
            int len)
        {
            float a = math.PI / len;
            for (int i = 0; i < len; ++i)
            {
                float sum = 0f;
                for (int j = 0; j < len; ++j)
                {
                    float ang = (j + 0.5f) * i * a;
                    sum += spectrum[j] * math.cos(ang);
                }
                cepstrum[i] = sum;
            }
        }

        // 计算信号的二范数,用于量化信号的整体能量
        public static float Norm(in NativeArray<float> array)
        {
            return Norm((float*)array.GetUnsafeReadOnlyPtr(), array.Length);
        }

        public static float Norm(in NativeSlice<float> slice)
        {
            return Norm((float*)slice.GetUnsafeReadOnlyPtr(), slice.Length);
        }

        [BurstCompile]
        static float Norm(float* array, int len)
        {
            float sum = 0f;
            for (int i = 0; i < len; ++i)
            {
                float x = array[i];
                sum += x * x;
            }
            return math.sqrt(sum);
        }
    }
}