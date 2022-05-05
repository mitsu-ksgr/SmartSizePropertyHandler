Windows: SmartSize PropertyHandler
==================================

みかん 🍊

### あれこれ
#### やりたかったこと
- Widnows Exprorer で詳細表示したときのサイズが KB 表示がいや
- サイズに応じて GiB, MiB の単位にしてほしい

#### やったこと
- サイズに応じて単位を使い分ける PropertyHandler を実装
- それ用の propdesc を用意

#### 結果
- 実装後に「PropertyHandler は各拡張子につき１つ」という仕様を知って泣いた
- 自作 PropertyHandler を登録すると、デフォルトのやつが無効になってしまう
  - mp4 だと、「長さ」とかがみれなくなる
- 悲しい。


### Directries
- `outputs`
    - `PropSchemaRegister.exe` ... propdesc を登録するやつ
    - `SmartSize.propdesc` ... propdesc file
        - `SmartSizePropertyHandler/SmartSize.propdesc` と同じ
        - PropSchemaRegister.exe で登録する
    - `SmartSizePropertyHandler.dll` ... dll
        - mp4 を対象にした状態でビルドしてある
        - けど、そのままだと mp4 は機能しない
        - レジストリエディタで SystemPropertyHandlers を編集する必要がある
- `PropDumper`
    - 勉強目的で MS のサンプルを再実装したやつ
        - https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/propertyschemas
        - ↑からregister機能をなくしたもの
- `PropSchemaRegister`
    - propdesc を登録するためのやつ
    - PropDumper のサンプルの register 機能を抜粋しただけ
- `SmartSizePropertyHandler`
    - PropertyHandler の実装
    - 基本的に MS のサンプルを眺めながら実装
        - https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appshellintegration/RecipePropertyHandler


## Note
### 前提知識
- PropertyHandler
    - ファイルからプロパティを取り出す
    - PropertyHandler は、各拡張子につき１個しか登録できない。悲しい。
- propdesc
    - プロパティの表示方法まわりを指定
    - これを登録しないと詳細表示のタブとかで使えない
- registry
    - SystemPropertyHandlers
        - key: `コンピューター\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\SystemPropertyHandlers`
        - 編集するにはアクセス許可を変更する必要がある
    - PropertyHandlers
        - key: `コンピューター\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers`
    - SystemPropertyHandlers が優先される


### PropertyHandler の登録方法
#### Step 1
- Powershell を管理者権限で実行
- このディレクトリまで移動
- PropSchemaRegister.exe で propdesc を登録
    - `> .\PropSchemaRegister.exe r .\SmartSize.propdesc`
    - 詳細表示のタブ表示に必要
- regsvr32 で dll を登録
    - `> regsvr32 .\SmartSizePropertyHandler.dll`
    - 登録解除は `regsvr32 /u hoge.dll`

#### Step 2
- レジストリエディタを起動
- SystemPropertyHandlers を編集
    - key: `コンピューター\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\SystemPropertyHandlers`
    - 対象の拡張子のハンドラを変更する
        - `{E5E96240-C9EE-11EC-9D64-0242AC120002}` に変更
        - もとの PropertyHandler の機能は失われるので注意
            - もとの uuid に戻せばおｋ

#### ちなみに
- dll ビルド時に指定した拡張子じゃなくても、手動登録すればおｋ
    - 拡張子がSystemPropertyHandlers に…
        - ある → SystemPropertyHandlers に登録されてるやつを編集
        - ない → PropertyHandlers に追加
            - `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers`

##### キーめも
- SmartSize PropertyHandler
    - key: `{E5E96240-C9EE-11EC-9D64-0242AC120002}`
- Default SystemPropertyHandler
    - mp4: `{f81b1b56-7613-4ee4-bc05-1fab5de5c07e}`
    - mkv: `{C591F150-4106-4141-B5C1-30B2101453BD}`

