using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Text;
using WaveGenEditor.Properties;

namespace WaveGenEditor
{
    /// <summary>
    /// スタートアップフォーム
    /// </summary>
    public partial class frmMain : Form
    {
        [System.Runtime.InteropServices.DllImport("User32.Dll")]
        private static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

        private WaveGenIF waveGen = null;
        private string filePath = null;
        private bool isWaveSave = false;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public frmMain()
        {
            InitializeComponent();
        }

        /// <summary>
        /// フォームロード
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_Load(object sender, EventArgs e)
        {
            rchMML.LanguageOption = RichTextBoxLanguageOptions.UIFonts;
            rchMML.Font = new Font( Resources.DefaultFontName,9);
            waveGen = new WaveGenIF();
            waveGen.CreateWaveGen(Handle);
        }

        /// <summary>
        /// ショートカットキーハンドリング
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="keyData"></param>
        /// <returns></returns>
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            // F5 キーは[Play]
            if ((int)keyData == (int)Keys.F5 )
            {
                cmdPlay.Checked = !cmdPlay.Checked;
                return true;
            } 
            return base.ProcessCmdKey(ref msg, keyData);
        }

        /// <summary>
        /// [開く]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenFile_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != cmnOpenDlg.ShowDialog())
                return;

            rchMML.Text = File.ReadAllText( cmnOpenDlg.FileName, Encoding.GetEncoding("shift_jis") );
            cmnSaveDlg.FileName = filePath = cmnOpenDlg.FileName;
            stsNotify.Text = Resources.Loaded;
        }

        /// <summary>
        /// [保存]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdSaveFile_Click(object sender, EventArgs e)
        {
            if (cmnOpenDlg.FileName.Trim() == ""){
                cmdSaveAsFile.PerformClick();
            }

            try
            {
                File.WriteAllText(cmnSaveDlg.FileName, rchMML.Text, Encoding.GetEncoding("shift_jis") );
                stsNotify.Text = Resources.Saved;
            }
            catch (Exception)
            {
            }
        }
        
        /// <summary>
        /// [名前を付けて保存]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdSaveAsFile_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != cmnSaveDlg.ShowDialog())
                return;
            cmnSaveDlg.FileName = cmnOpenDlg.FileName = cmnSaveDlg.FileName;
        }

        /// <summary>
        /// [終了]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdAppExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// [Web]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenGithub_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen");
        }

        /// <summary>
        /// [Web]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenDL_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen/downloads");
        }

        private void cmdOpenMMLref_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen/wiki/MML");
        }

        /// <summary>
        /// フォーム破棄
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
 
        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (waveGen != null)
            {
                waveGen.Release();
                waveGen = null;
            }
        }

        /// <summary>
        /// 再生
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdPlay_CheckedChanged(object sender, EventArgs e)
        {
            if (cmdPlay.Checked)
            {
                if (chkAutoSave.Checked && isWaveSave==false )
                    cmdSaveFile.PerformClick();

                cmdPlay.Text = Resources.StopButtonText + " (F5)";
                Int32 errorCode = 0;
                UInt32 errorLine = 0;
                if (waveGen.CompileMML(rchMML.Text, ref errorCode, ref errorLine, isWaveSave, dlgPlayFinished_, this))
                {
                    stsNotify.Text = Resources.MMLerror.Replace( "_LINE_", errorLine.ToString() ).Replace( "_MSG_", WaveGenIF.GetErrorString(errorCode));
                    MoveLine((int)errorLine);
                    System.Media.SystemSounds.Exclamation.Play();
                    cmdPlay.Checked = false;
                    return;
                }
                stsNotify.Text = "";
            }else{
                if (waveGen != null)
                    waveGen.Stop();
                cmdPlay.Text = Resources.PlayButtonText + " (F5)";
            }
        }
        static void dlgPlayFinished_(WaveGenIF WaveGen, object parent)
        {
            frmMain pThis = (frmMain)parent;
            pThis.dlgPlayfinished(WaveGen);
        }
        delegate void dlgPlayfinishedInvoke(WaveGenIF WaveGen);
        void dlgPlayfinished( WaveGenIF WaveGen )
        {
            BeginInvoke((MethodInvoker)delegate
            {
                if (cmdPlay.Checked)
                    cmdPlay.Checked = false;
            });   
        }

        /// <summary>
        /// 指定行までジャンプ
        /// </summary>
        /// <param name="line">ジャンプする行</param>
        private void MoveLine(int line)
        {
            string str = rchMML.Text;

            int row=1, startPos=0, endPos;
            for (endPos = 0;(endPos = str.IndexOf('\n', startPos)) > -1; row++)
            {
                if (line == row)
                {
                    rchMML.SelectionStart = startPos;
                    return;
                }
                       
                startPos = endPos + 1;
            }
        }

        /// <summary>
        /// WaveFileに録音する
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdWriteWaveFile_Click(object sender, EventArgs e)
        {
            if (cmdPlay.Checked)
                cmdPlay.Checked = false;

            if (DialogResult.OK != cmnWavSaveDlg.ShowDialog())
                return;

            waveGen.SetWaveFileName(cmnWavSaveDlg.FileName);

            isWaveSave = true;
            cmdPlay.Checked = true;
            isWaveSave = false;
        }
    }
}
