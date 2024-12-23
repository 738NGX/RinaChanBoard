using UnityEngine;
using UnityEditor;
using UnityEditorInternal;
using System.IO;
using System.Collections.Generic;
using System.Text;
using RinaLipSync.Debugging;

namespace RinaLipSync
{

[CustomEditor(typeof(Profile))]
public class ProfileEditor : Editor
{
    Profile profile => target as Profile;
    public RinaLipSync rinaLipSync { get; set; }
    bool _isCalibrating = false;
    ReorderableList _reorderableList = null;
    readonly Dictionary<MfccData, Texture2D> _texturePool = new();

    void OnEnable()
    {
    }

    public override void OnInspectorGUI()
    {
        Draw(false);
    }

    public void Draw(bool showCalibration)
    {
        serializedObject.Update();

        if (EditorUtil.SimpleFoldout("MFCC", true, "-rinaLipSync-Profile"))
        {
            EditorGUI.BeginChangeCheck();
            
            ++EditorGUI.indentLevel;
            DrawMfccReorderableList(showCalibration);
            --EditorGUI.indentLevel;
            
            if (EditorGUI.EndChangeCheck())
            {
                EditorUtility.SetDirty(target);
            }
        }

        if (EditorUtil.SimpleFoldout("Advanced Parameters", true, "-rinaLipSync-Profile"))
        {
            ++EditorGUI.indentLevel;
            EditorUtil.DrawProperty(serializedObject, nameof(profile.mfccDataCount));
            EditorUtil.DrawProperty(serializedObject, nameof(profile.melFilterBankChannels));
            EditorUtil.DrawProperty(serializedObject, nameof(profile.targetSampleRate));
            EditorUtil.DrawProperty(serializedObject, nameof(profile.sampleCount));
            bool useStandardization = EditorGUILayout.Toggle("Use Standardization", profile.useStandardization);
            if (useStandardization != profile.useStandardization)
            {
                Undo.RecordObject(target, "Change Use Standardization");
                profile.useStandardization = useStandardization;
                profile.UpdateMeansAndStandardization();
                EditorUtility.SetDirty(target);
            }
            EditorUtil.DrawProperty(serializedObject, nameof(profile.compareMethod));
            profile.mfccDataCount = Mathf.Clamp(profile.mfccDataCount, 1, 256);
            profile.melFilterBankChannels = Mathf.Clamp(profile.melFilterBankChannels, 12, 256);
            profile.targetSampleRate = Mathf.Clamp(profile.targetSampleRate, 1000, 96000);
            profile.sampleCount = Mathf.ClosestPowerOfTwo(profile.sampleCount);
            --EditorGUI.indentLevel;

            EditorGUILayout.Separator();
        }

        if (EditorUtil.SimpleFoldout("Import / Export JSON", false, "-rinaLipSync-Profile"))
        {
            ++EditorGUI.indentLevel;
            DrawImportExport();
            --EditorGUI.indentLevel;

            EditorGUILayout.Separator();
        }

        if (EditorUtil.SimpleFoldout("Save", false, "-rinaLipSync-Profile"))
        {
            ++EditorGUI.indentLevel;
            DrawSave();
            --EditorGUI.indentLevel;

            EditorGUILayout.Separator();
        }

#if RINALIPSYNC_DEBUG
        if (EditorUtil.SimpleFoldout("Debug", false, "-rinaLipSync-Profile"))
        {
            ++EditorGUI.indentLevel;
            DrawDebug();
            --EditorGUI.indentLevel;

            EditorGUILayout.Separator();
        }
#endif

        if (EditorUtil.SimpleFoldout("Baked Data", false, "-rinaLipSync-Profile"))
        {
            ++EditorGUI.indentLevel;
            --EditorGUI.indentLevel;

            EditorGUILayout.Separator();
        }

        serializedObject.ApplyModifiedProperties();
    }

    void DrawMfccReorderableList(bool showCalibration)
    {
        if (_reorderableList == null)
        {
            _reorderableList = new ReorderableList(profile.mfccs, typeof(MfccData));
            _reorderableList.drawHeaderCallback = rect => 
            {
                rect.xMin -= EditorGUI.indentLevel * 12f;
                EditorGUI.LabelField(rect, "MFCCs");
            };
            _reorderableList.draggable = true;
            _reorderableList.drawElementCallback = (rect, index, isActive, isFocused) =>
            {
                DrawMFCC(rect, index, showCalibration);
            };
            _reorderableList.elementHeightCallback = index =>
            {
                return GetMFCCHeight(index);
            };
            _reorderableList.onAddCallback = index =>
            {
                profile.AddMfcc("New Phoneme");
            };
            _reorderableList.onRemoveCallback = list =>
            {
                profile.RemoveMfcc(list.index);
            };
        }

        EditorGUILayout.Separator();
        EditorGUILayout.BeginHorizontal();
        var indent = EditorGUI.indentLevel * 12f;
        EditorGUILayout.Space(indent, false);
        EditorGUILayout.BeginVertical();
        _reorderableList.DoLayoutList();
        EditorGUILayout.EndVertical();
        EditorGUILayout.EndHorizontal();
    }

    void DrawMFCC(Rect position, int index, bool showCalibration)
    {
        var data = profile.mfccs[index];

        position = EditorGUI.IndentedRect(position);
        position.xMin += 12;
        position.height = EditorUtil.lineHeightWithMargin;

        if (!EditorUtil.SimpleFoldout(position, data.name, true, "MfccData")) return;
        position.y += EditorUtil.lineHeightWithMargin;

        var newName = EditorGUI.TextField(position, "Phoneme", data.name);
        if (newName != data.name)
        {
            Undo.RecordObject(target, "Change Phoneme");
            data.name = newName;
            EditorUtility.SetDirty(target);
        }
        position.y += EditorUtil.lineHeightWithMargin;
        position.y += 5f;

        var mfccPos = new Rect(position);
        if (showCalibration)
        {
            mfccPos.xMax -= 60;
        }

        if (!_texturePool.TryGetValue(data, out Texture2D tex)) tex = null;
        tex = TextureCreator.CreateMfccTexture(tex, data, Common.MfccMinValue, Common.MfccMaxValue);
        _texturePool[data] = tex;
        
        var area = EditorGUI.IndentedRect(mfccPos);
        area.height = data.mfccCalibrationDataList.Count * 3f;
        GUI.DrawTexture(area, tex, ScaleMode.StretchToFill);

        var calibButtonPos = new Rect(position);
        calibButtonPos.xMin = mfccPos.xMax + 8;
        calibButtonPos.height = 2f * data.mfccCalibrationDataList.Count;
        if (showCalibration)
        {
            var text = new GUIContent(" Calib ");
            var e = Event.current;
            if (e != null && calibButtonPos.Contains(e.mousePosition))
            {
                if (e.type == EventType.MouseDown)
                {
                    _isCalibrating = true;
                }
                else if (e.type == EventType.MouseUp)
                {
                    _isCalibrating = false;
                }

                if (_isCalibrating)
                {
                    rinaLipSync.RequestCalibration(index);
                }

                Repaint();
            }
            else if (e.isMouse)
            {
                _isCalibrating = false;
            }

            var style = new GUIStyle(GUI.skin.button);
            style.fixedHeight = 20f;
            GUI.Button(calibButtonPos, text, style);
        }
    }

    float GetMFCCHeight(int index)
    {
        string name = "";
        float height = 20f;

        if (index < profile.mfccs.Count)
        {
            var data = profile.mfccs[index];
            name = data.name;

            if (EditorUtil.IsFoldOutOpened(name, true, "MfccData"))
            {
                height += 32f;
                height += data.mfccCalibrationDataList.Count * 3f;
            }
        }

        return height;
    }

    void DrawImportExport()
    {
        EditorGUILayout.BeginHorizontal();
        var newJsonPath = EditorGUILayout.TextField("File Path", profile.jsonPath);
        if (newJsonPath != profile.jsonPath)
        {
            Undo.RecordObject(target, "Change File Path");
            profile.jsonPath = newJsonPath;
            EditorUtility.SetDirty(target);
        }
        if (GUILayout.Button("...", EditorStyles.miniButton, GUILayout.Width(24)))
        {
            try
            {
                var dir = Path.GetDirectoryName(profile.jsonPath);
                var file = Path.GetFileName(profile.jsonPath);
                profile.jsonPath = EditorUtility.SaveFilePanel("Select Profile", dir, file, "json");
            }
            catch
            {
                profile.jsonPath = EditorUtility.SaveFilePanel("Select Profile", "", "profile", "json");
                profile.Export(profile.jsonPath);
            }
        }
        EditorGUILayout.EndHorizontal();

        EditorGUILayout.BeginHorizontal();
        GUILayout.FlexibleSpace();
        if (GUILayout.Button("  Import  ", EditorStyles.miniButtonLeft))
        {
            profile.Import(profile.jsonPath);
        }
        if (GUILayout.Button("  Export  ", EditorStyles.miniButtonRight))
        {
            profile.Export(profile.jsonPath);
        }
        EditorGUILayout.EndHorizontal();
    }

    void DrawSave()
    {
        EditorGUILayout.BeginHorizontal();
        GUILayout.FlexibleSpace();
        if (GUILayout.Button("  Save  "))
        {
            profile.Save();
        }
        EditorGUILayout.EndHorizontal();
    }

    void DrawDebug()
    {
        EditorGUILayout.BeginHorizontal();
        GUILayout.FlexibleSpace();
        if (GUILayout.Button("  Dump  "))
        {
            var profileName = string.IsNullOrEmpty(profile.name) ? "profile" : profile.name;
            foreach (var mfcc in profile.mfccs)
            {
                var filename = $"{profileName}-{mfcc.name}.csv";
                var sw = new StreamWriter(filename);
                var sb = new StringBuilder();
                DebugUtil.DumpMfccData(sb, mfcc);
                sw.Write(sb);
                sw.Close();
                Debug.Log($"{filename} was created.");
            }
        }
        EditorGUILayout.EndHorizontal();
    }
}

}
