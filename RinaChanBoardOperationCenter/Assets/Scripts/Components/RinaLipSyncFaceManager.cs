using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RinaLipSync
{
    public class RinaLipSyncFaceManager : MonoBehaviour
    {
        public UpdateMethod updateMethod = UpdateMethod.LipSyncUpdateEvent;
        public float minVolume = -2.5f;
        [Range(0f, 1f)] public float minDuration = 0.1f;

        private UdpServer udpServer;
        private readonly Dictionary<string, Face> faceDict = new();
        private readonly List<string> phonemeHistory = new();
        private readonly Dictionary<string, int> phonemeCountTable = new();
        private LipSyncInfo info = new();
        private string phoneme = "";
        private string lastPhoneme = "N";
        private float keepTimer = 0f;
        private bool lipSyncUpdated = false;
        private float volume = -100f;
        private bool mouthClosed = true;

        private void Start()
        {
            udpServer = GameObject.Find("UDP Server").GetComponent<UdpServer>();
            faceDict["A"] = new Face(101, 201, 311, 400);
            faceDict["E"] = new Face(101, 201, 323, 400);
            faceDict["I"] = new Face(101, 201, 321, 400);
            faceDict["O"] = new Face(101, 201, 312, 400);
            faceDict["U"] = new Face(101, 201, 319, 400);
            faceDict["N"] = new Face(101, 201, 302, 400);
        }

        public void OnLipSyncUpdate(LipSyncInfo info)
        {
            this.info = info;
            lipSyncUpdated = true;

            if (updateMethod == UpdateMethod.LipSyncUpdateEvent)
            {
                UpdateLipSync();
                Apply();
            }
        }

        private void Update()
        {
            if (updateMethod != UpdateMethod.LipSyncUpdateEvent)
            {
                UpdateLipSync();
            }

            if (updateMethod == UpdateMethod.Update)
            {
                Apply();
            }
        }

        private void UpdateLipSync()
        {
            UpdateVolume();
            UpdateVowels();
            lipSyncUpdated = false;
        }

        private void UpdateVolume()
        {
            volume = 0f;

            if (!lipSyncUpdated) return;

            if (info.rawVolume > 0f)
            {
                volume = Mathf.Log10(info.rawVolume);
            }
        }

        private void UpdateVowels()
        {
            if (lipSyncUpdated && volume > minVolume)
            {
                phonemeHistory.Add(info.phoneme);
            }
            else
            {
                phonemeHistory.Add("");
            }

            int minFrame = (int)Mathf.Max(minDuration / Time.deltaTime, 1f);
            while (phonemeHistory.Count > minFrame)
            {
                phonemeHistory.RemoveAt(0);
            }

            phonemeCountTable.Clear();

            foreach (var key in phonemeHistory)
            {
                if (phonemeCountTable.ContainsKey(key))
                {
                    ++phonemeCountTable[key];
                }
                else
                {
                    phonemeCountTable.Add(key, 0);
                }
            }

            keepTimer += Time.deltaTime;
            if (keepTimer < minDuration) return;

            int maxCount = 0;
            string phoneme = "";
            foreach (var kv in phonemeCountTable)
            {
                if (kv.Value > maxCount)
                {
                    phoneme = kv.Key;
                    maxCount = kv.Value;
                }
            }

            if (this.phoneme != phoneme)
            {
                keepTimer = 0f;
                this.phoneme = phoneme;
            }
        }

        private void Apply()
        {
            int eye = Random.Range(0, 10) switch
            {
                0 => 2,
                1 => 6,
                2 => 7,
                3 => 10,
                4 => 11,
                _ => 1
            };
            
            if (volume == 0 || volume < minVolume)
            {
                if (mouthClosed) return;
                var face = faceDict["N"];
                udpServer.LEDs.UpdateFace(face);
                if (udpServer.SendLiteFace)
                    udpServer.UploadFaceLite(face);
                else
                    udpServer.UploadFace();
                mouthClosed = true;
                return;
            }
            if (isActiveAndEnabled && phoneme != lastPhoneme && faceDict.ContainsKey(phoneme))
            {
                if (mouthClosed) mouthClosed = false;
                var face = new Face(100 + eye, 200 + eye, faceDict[phoneme].Mouth, 400);
                udpServer.LEDs.UpdateFace(face);
                if (udpServer.SendLiteFace)
                    udpServer.UploadFaceLite(face);
                else
                    udpServer.UploadFace();
                lastPhoneme = phoneme;
            }
        }
    }
}