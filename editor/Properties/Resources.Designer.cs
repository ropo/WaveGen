﻿//------------------------------------------------------------------------------
// <auto-generated>
//     このコードはツールによって生成されました。
//     ランタイム バージョン:4.0.30319.261
//
//     このファイルへの変更は、以下の状況下で不正な動作の原因になったり、
//     コードが再生成されるときに損失したりします。
// </auto-generated>
//------------------------------------------------------------------------------

namespace WaveGenEditor.Properties {
    using System;
    
    
    /// <summary>
    ///   ローカライズされた文字列などを検索するための、厳密に型指定されたリソース クラスです。
    /// </summary>
    // このクラスは StronglyTypedResourceBuilder クラスが ResGen
    // または Visual Studio のようなツールを使用して自動生成されました。
    // メンバーを追加または削除するには、.ResX ファイルを編集して、/str オプションと共に
    // ResGen を実行し直すか、または VS プロジェクトをビルドし直します。
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   このクラスで使用されているキャッシュされた ResourceManager インスタンスを返します。
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("WaveGenEditor.Properties.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   厳密に型指定されたこのリソース クラスを使用して、すべての検索リソースに対し、
        ///   現在のスレッドの CurrentUICulture プロパティをオーバーライドします。
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   ＭＳ ゴシック に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string DefaultFontName {
            get {
                return ResourceManager.GetString("DefaultFontName", resourceCulture);
            }
        }
        
        /// <summary>
        ///   読み込みました に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string Loaded {
            get {
                return ResourceManager.GetString("Loaded", resourceCulture);
            }
        }
        
        /// <summary>
        ///   ＭＭＬ構文エラー：_LINE_行目【_MSG_】 に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string MMLerror {
            get {
                return ResourceManager.GetString("MMLerror", resourceCulture);
            }
        }
        
        /// <summary>
        ///   再生 に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string PlayButtonText {
            get {
                return ResourceManager.GetString("PlayButtonText", resourceCulture);
            }
        }
        
        /// <summary>
        ///   保存しました に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string Saved {
            get {
                return ResourceManager.GetString("Saved", resourceCulture);
            }
        }
        
        /// <summary>
        ///   停止 に類似しているローカライズされた文字列を検索します。
        /// </summary>
        internal static string StopButtonText {
            get {
                return ResourceManager.GetString("StopButtonText", resourceCulture);
            }
        }
    }
}
