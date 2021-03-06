﻿namespace WaveGenEditor
{
    partial class frmMain
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.stsBar = new System.Windows.Forms.StatusStrip();
            this.stsNotify = new System.Windows.Forms.ToolStripStatusLabel();
            this.cmnOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.menuMain = new System.Windows.Forms.MenuStrip();
            this.cmdFile = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdOpenFile = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdSaveFile = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdSaveAsFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdWriteWaveFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdAppExit = new System.Windows.Forms.ToolStripMenuItem();
            this.表示VToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdPreviewPiano = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdParamEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdADSRParamEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdFMParamEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdOpenWeb = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdOpenGithub = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdOpenDL = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdOpenMMLref = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnSaveDlg = new System.Windows.Forms.SaveFileDialog();
            this.rchMML = new System.Windows.Forms.RichTextBox();
            this.ctmEdit = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.cmdEditCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdEditPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdEditCut = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdMenuPlay = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdPlay = new System.Windows.Forms.CheckBox();
            this.chkAutoSave = new System.Windows.Forms.CheckBox();
            this.cmnWavSaveDlg = new System.Windows.Forms.SaveFileDialog();
            this.keyPreview = new WaveGenEditor.KeybordControl();
            this.stsBar.SuspendLayout();
            this.menuMain.SuspendLayout();
            this.ctmEdit.SuspendLayout();
            this.SuspendLayout();
            // 
            // stsBar
            // 
            this.stsBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.stsNotify});
            this.stsBar.Location = new System.Drawing.Point(0, 417);
            this.stsBar.Name = "stsBar";
            this.stsBar.Size = new System.Drawing.Size(574, 22);
            this.stsBar.TabIndex = 2;
            this.stsBar.Text = "statusStrip1";
            // 
            // stsNotify
            // 
            this.stsNotify.Name = "stsNotify";
            this.stsNotify.Size = new System.Drawing.Size(559, 17);
            this.stsNotify.Spring = true;
            this.stsNotify.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmnOpenDlg
            // 
            this.cmnOpenDlg.Filter = "MMLファイル|*.mml|すべてのファイル|*.*";
            // 
            // menuMain
            // 
            this.menuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdFile,
            this.表示VToolStripMenuItem,
            this.cmdParamEdit,
            this.cmdOpenWeb});
            this.menuMain.Location = new System.Drawing.Point(0, 0);
            this.menuMain.Name = "menuMain";
            this.menuMain.Size = new System.Drawing.Size(574, 26);
            this.menuMain.TabIndex = 4;
            this.menuMain.Text = "menuStrip1";
            // 
            // cmdFile
            // 
            this.cmdFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdOpenFile,
            this.cmdSaveFile,
            this.cmdSaveAsFile,
            this.toolStripMenuItem3,
            this.cmdWriteWaveFile,
            this.toolStripMenuItem2,
            this.cmdAppExit});
            this.cmdFile.Name = "cmdFile";
            this.cmdFile.Size = new System.Drawing.Size(85, 22);
            this.cmdFile.Text = "ファイル(&F)";
            // 
            // cmdOpenFile
            // 
            this.cmdOpenFile.Name = "cmdOpenFile";
            this.cmdOpenFile.ShortcutKeyDisplayString = "Ctrl+O";
            this.cmdOpenFile.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.cmdOpenFile.Size = new System.Drawing.Size(374, 22);
            this.cmdOpenFile.Text = "開く(&O)...";
            this.cmdOpenFile.Click += new System.EventHandler(this.cmdOpenFile_Click);
            // 
            // cmdSaveFile
            // 
            this.cmdSaveFile.Name = "cmdSaveFile";
            this.cmdSaveFile.ShortcutKeyDisplayString = "Ctrl+S";
            this.cmdSaveFile.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.cmdSaveFile.Size = new System.Drawing.Size(374, 22);
            this.cmdSaveFile.Text = "保存(&S)...";
            this.cmdSaveFile.Click += new System.EventHandler(this.cmdSaveFile_Click);
            // 
            // cmdSaveAsFile
            // 
            this.cmdSaveAsFile.Name = "cmdSaveAsFile";
            this.cmdSaveAsFile.ShortcutKeyDisplayString = "Ctrl+Shift+S";
            this.cmdSaveAsFile.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.cmdSaveAsFile.Size = new System.Drawing.Size(374, 22);
            this.cmdSaveAsFile.Text = "名前を付けて保存(&A)...";
            this.cmdSaveAsFile.Click += new System.EventHandler(this.cmdSaveAsFile_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(371, 6);
            // 
            // cmdWriteWaveFile
            // 
            this.cmdWriteWaveFile.Name = "cmdWriteWaveFile";
            this.cmdWriteWaveFile.ShortcutKeyDisplayString = "Ctrl+Shift+W";
            this.cmdWriteWaveFile.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.W)));
            this.cmdWriteWaveFile.Size = new System.Drawing.Size(374, 22);
            this.cmdWriteWaveFile.Text = "Waveファイルにレコーディング(&W)...";
            this.cmdWriteWaveFile.Click += new System.EventHandler(this.cmdWriteWaveFile_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(371, 6);
            // 
            // cmdAppExit
            // 
            this.cmdAppExit.Name = "cmdAppExit";
            this.cmdAppExit.ShortcutKeyDisplayString = "Alt+F4";
            this.cmdAppExit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F4)));
            this.cmdAppExit.Size = new System.Drawing.Size(374, 22);
            this.cmdAppExit.Text = "終了(&X)";
            this.cmdAppExit.Click += new System.EventHandler(this.cmdAppExit_Click);
            // 
            // 表示VToolStripMenuItem
            // 
            this.表示VToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdPreviewPiano});
            this.表示VToolStripMenuItem.Name = "表示VToolStripMenuItem";
            this.表示VToolStripMenuItem.Size = new System.Drawing.Size(62, 22);
            this.表示VToolStripMenuItem.Text = "表示(&V)";
            // 
            // cmdPreviewPiano
            // 
            this.cmdPreviewPiano.Checked = true;
            this.cmdPreviewPiano.CheckOnClick = true;
            this.cmdPreviewPiano.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cmdPreviewPiano.Name = "cmdPreviewPiano";
            this.cmdPreviewPiano.Size = new System.Drawing.Size(172, 22);
            this.cmdPreviewPiano.Text = "プレビューピアノ";
            this.cmdPreviewPiano.CheckedChanged += new System.EventHandler(this.cmdPreviewPiano_CheckedChanged);
            // 
            // cmdParamEdit
            // 
            this.cmdParamEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdADSRParamEdit,
            this.cmdFMParamEdit});
            this.cmdParamEdit.Name = "cmdParamEdit";
            this.cmdParamEdit.Size = new System.Drawing.Size(145, 22);
            this.cmdParamEdit.Text = "パラメータエディタ(&P)";
            // 
            // cmdADSRParamEdit
            // 
            this.cmdADSRParamEdit.Name = "cmdADSRParamEdit";
            this.cmdADSRParamEdit.Size = new System.Drawing.Size(152, 22);
            this.cmdADSRParamEdit.Text = "ADSR";
            this.cmdADSRParamEdit.Click += new System.EventHandler(this.cmdADSRParamEdit_Click);
            // 
            // cmdFMParamEdit
            // 
            this.cmdFMParamEdit.Name = "cmdFMParamEdit";
            this.cmdFMParamEdit.Size = new System.Drawing.Size(152, 22);
            this.cmdFMParamEdit.Text = "FM";
            this.cmdFMParamEdit.Visible = false;
            // 
            // cmdOpenWeb
            // 
            this.cmdOpenWeb.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdOpenGithub,
            this.cmdOpenDL,
            this.toolStripMenuItem1,
            this.cmdOpenMMLref});
            this.cmdOpenWeb.Name = "cmdOpenWeb";
            this.cmdOpenWeb.Size = new System.Drawing.Size(103, 22);
            this.cmdOpenWeb.Text = "Webを開く(&W)";
            // 
            // cmdOpenGithub
            // 
            this.cmdOpenGithub.Name = "cmdOpenGithub";
            this.cmdOpenGithub.Size = new System.Drawing.Size(301, 22);
            this.cmdOpenGithub.Text = "プロジェクトトップページを表示(&P)...";
            this.cmdOpenGithub.Click += new System.EventHandler(this.cmdOpenGithub_Click);
            // 
            // cmdOpenDL
            // 
            this.cmdOpenDL.Name = "cmdOpenDL";
            this.cmdOpenDL.Size = new System.Drawing.Size(301, 22);
            this.cmdOpenDL.Text = "最新版ダウンロードページを表示(&N)...";
            this.cmdOpenDL.Click += new System.EventHandler(this.cmdOpenDL_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(298, 6);
            // 
            // cmdOpenMMLref
            // 
            this.cmdOpenMMLref.Name = "cmdOpenMMLref";
            this.cmdOpenMMLref.ShortcutKeyDisplayString = "F1";
            this.cmdOpenMMLref.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.cmdOpenMMLref.Size = new System.Drawing.Size(301, 22);
            this.cmdOpenMMLref.Text = "MMLリファレンスページを表示(&M)...";
            this.cmdOpenMMLref.Click += new System.EventHandler(this.cmdOpenMMLref_Click);
            // 
            // cmnSaveDlg
            // 
            this.cmnSaveDlg.Filter = "MMLファイル|*.mml|すべてのファイル|*.*";
            // 
            // rchMML
            // 
            this.rchMML.AcceptsTab = true;
            this.rchMML.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rchMML.ContextMenuStrip = this.ctmEdit;
            this.rchMML.Location = new System.Drawing.Point(0, 74);
            this.rchMML.Name = "rchMML";
            this.rchMML.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedBoth;
            this.rchMML.Size = new System.Drawing.Size(574, 340);
            this.rchMML.TabIndex = 0;
            this.rchMML.Text = "t128\n\nTrack(0)\n@0 v100 l8 q90 o4  ADSR( 1, 0.01, 0, 1, 0.01 )\n";
            // 
            // ctmEdit
            // 
            this.ctmEdit.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdEditCopy,
            this.cmdEditPaste,
            this.cmdEditCut,
            this.toolStripSeparator1,
            this.cmdMenuPlay});
            this.ctmEdit.Name = "ctmEdit";
            this.ctmEdit.Size = new System.Drawing.Size(189, 98);
            // 
            // cmdEditCopy
            // 
            this.cmdEditCopy.Name = "cmdEditCopy";
            this.cmdEditCopy.ShortcutKeyDisplayString = "Ctrl+C";
            this.cmdEditCopy.Size = new System.Drawing.Size(188, 22);
            this.cmdEditCopy.Text = "コピー(&C)";
            this.cmdEditCopy.Click += new System.EventHandler(this.cmdEditCopy_Click);
            // 
            // cmdEditPaste
            // 
            this.cmdEditPaste.Name = "cmdEditPaste";
            this.cmdEditPaste.ShortcutKeyDisplayString = "Ctrl+V";
            this.cmdEditPaste.Size = new System.Drawing.Size(188, 22);
            this.cmdEditPaste.Text = "ペースト(&P)";
            this.cmdEditPaste.Click += new System.EventHandler(this.cmdEditPaste_Click);
            // 
            // cmdEditCut
            // 
            this.cmdEditCut.Name = "cmdEditCut";
            this.cmdEditCut.ShortcutKeyDisplayString = "Ctrl+X";
            this.cmdEditCut.Size = new System.Drawing.Size(188, 22);
            this.cmdEditCut.Text = "カット(&C)";
            this.cmdEditCut.Click += new System.EventHandler(this.cmdEditCut_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(185, 6);
            // 
            // cmdMenuPlay
            // 
            this.cmdMenuPlay.Name = "cmdMenuPlay";
            this.cmdMenuPlay.ShortcutKeyDisplayString = "F5";
            this.cmdMenuPlay.Size = new System.Drawing.Size(188, 22);
            this.cmdMenuPlay.Text = "再生・停止";
            this.cmdMenuPlay.Click += new System.EventHandler(this.cmdMenuPlay_Click);
            // 
            // cmdPlay
            // 
            this.cmdPlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdPlay.Appearance = System.Windows.Forms.Appearance.Button;
            this.cmdPlay.AutoSize = true;
            this.cmdPlay.Location = new System.Drawing.Point(498, 29);
            this.cmdPlay.Name = "cmdPlay";
            this.cmdPlay.Size = new System.Drawing.Size(64, 22);
            this.cmdPlay.TabIndex = 2;
            this.cmdPlay.Text = "再生 (F5)";
            this.cmdPlay.UseVisualStyleBackColor = true;
            this.cmdPlay.CheckedChanged += new System.EventHandler(this.cmdPlay_CheckedChanged);
            // 
            // chkAutoSave
            // 
            this.chkAutoSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chkAutoSave.AutoSize = true;
            this.chkAutoSave.Location = new System.Drawing.Point(375, 33);
            this.chkAutoSave.Name = "chkAutoSave";
            this.chkAutoSave.Size = new System.Drawing.Size(117, 16);
            this.chkAutoSave.TabIndex = 1;
            this.chkAutoSave.Text = "再生時に自動保存";
            this.chkAutoSave.UseVisualStyleBackColor = true;
            // 
            // cmnWavSaveDlg
            // 
            this.cmnWavSaveDlg.Filter = "WAVEファイル|*.wav|すべてのファイル|*.*";
            // 
            // keyPreview
            // 
            this.keyPreview.BackColor = System.Drawing.Color.White;
            this.keyPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.keyPreview.KeysCount = ((byte)(88));
            this.keyPreview.Location = new System.Drawing.Point(12, 33);
            this.keyPreview.Name = "keyPreview";
            this.keyPreview.Size = new System.Drawing.Size(357, 35);
            this.keyPreview.TabIndex = 5;
            this.keyPreview.TabStop = false;
            this.keyPreview.EventNoteOn += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOn);
            this.keyPreview.EventNoteOff += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOff);
            this.keyPreview.EventChangeType += new WaveGenEditor.KeybordControl.EventHandlerChangeType(this.keyPreview_EventChangeType);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(574, 439);
            this.Controls.Add(this.keyPreview);
            this.Controls.Add(this.chkAutoSave);
            this.Controls.Add(this.cmdPlay);
            this.Controls.Add(this.rchMML);
            this.Controls.Add(this.stsBar);
            this.Controls.Add(this.menuMain);
            this.DoubleBuffered = true;
            this.MainMenuStrip = this.menuMain;
            this.MinimumSize = new System.Drawing.Size(590, 329);
            this.Name = "frmMain";
            this.Text = "WaveGenEditor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.stsBar.ResumeLayout(false);
            this.stsBar.PerformLayout();
            this.menuMain.ResumeLayout(false);
            this.menuMain.PerformLayout();
            this.ctmEdit.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip stsBar;
        private System.Windows.Forms.ToolStripStatusLabel stsNotify;
        private System.Windows.Forms.OpenFileDialog cmnOpenDlg;
        private System.Windows.Forms.MenuStrip menuMain;
        private System.Windows.Forms.ToolStripMenuItem cmdFile;
        private System.Windows.Forms.ToolStripMenuItem cmdOpenFile;
        private System.Windows.Forms.ToolStripMenuItem cmdSaveFile;
        private System.Windows.Forms.ToolStripMenuItem cmdSaveAsFile;
        private System.Windows.Forms.SaveFileDialog cmnSaveDlg;
        private System.Windows.Forms.ToolStripMenuItem cmdOpenWeb;
        private System.Windows.Forms.ToolStripMenuItem cmdOpenGithub;
        private System.Windows.Forms.ToolStripMenuItem cmdOpenDL;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem cmdOpenMMLref;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem cmdAppExit;
        private System.Windows.Forms.RichTextBox rchMML;
        private System.Windows.Forms.CheckBox cmdPlay;
        private System.Windows.Forms.CheckBox chkAutoSave;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem cmdWriteWaveFile;
        private System.Windows.Forms.SaveFileDialog cmnWavSaveDlg;
        private System.Windows.Forms.ToolStripMenuItem cmdParamEdit;
        private System.Windows.Forms.ToolStripMenuItem cmdADSRParamEdit;
        private System.Windows.Forms.ToolStripMenuItem cmdFMParamEdit;
        private System.Windows.Forms.ContextMenuStrip ctmEdit;
        private System.Windows.Forms.ToolStripMenuItem cmdEditCopy;
        private System.Windows.Forms.ToolStripMenuItem cmdEditPaste;
        private System.Windows.Forms.ToolStripMenuItem cmdEditCut;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem cmdMenuPlay;
        private KeybordControl keyPreview;
        private System.Windows.Forms.ToolStripMenuItem 表示VToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cmdPreviewPiano;
    }
}

