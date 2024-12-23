using UnityEngine;

namespace RinaLipSync
{
    [RequireComponent(typeof(AudioSource))]
    public class RinaLipSyncAudioSource : MonoBehaviour
    {
        public AudioFilterReadEvent onAudioFilterRead { get; private set; } = new AudioFilterReadEvent();

        private void OnAudioFilterRead(float[] input, int channels)
        {
            onAudioFilterRead?.Invoke(input, channels);
        }
    }
}
