Windows: SmartSize PropertyHandler
==================================

ã¿ãã ð

### ãããã
#### ç°å¢
- windows 10

#### ããããã£ããã¨
- Widnows Exprorer ã§è©³ç´°è¡¨ç¤ºããã¨ãã®ãµã¤ãºã KB è¡¨ç¤ºããã
- ãµã¤ãºã«å¿ãã¦ GiB, MiB ã®åä½ã«ãã¦ã»ãã

#### ãã£ããã¨
- ãµã¤ãºã«å¿ãã¦åä½ãä½¿ãåãã PropertyHandler ãå®è£
- ããç¨ã® propdesc ãç¨æ

#### çµæ
- å®è£å¾ã«ãPropertyHandler ã¯åæ¡å¼µå­ã«ã¤ãï¼ã¤ãã¨ããä»æ§ãç¥ã£ã¦æ³£ãã
- èªä½ PropertyHandler ãç»é²ããã¨ãããã©ã«ãã®ãã¤ãç¡å¹ã«ãªã£ã¦ãã¾ã
  - mp4 ã ã¨ããé·ããã¨ããã¿ããªããªã
- æ²ããã


### Directries
- `outputs`
    - `PropSchemaRegister.exe` ... propdesc ãç»é²ãããã¤
    - `SmartSize.propdesc` ... propdesc file
        - `SmartSizePropertyHandler/SmartSize.propdesc` ã¨åã
        - PropSchemaRegister.exe ã§ç»é²ãã
    - `SmartSizePropertyHandler.dll` ... dll
        - mp4 ãå¯¾è±¡ã«ããç¶æã§ãã«ããã¦ãã
        - ãã©ããã®ã¾ã¾ã ã¨ mp4 ã¯æ©è½ããªã
        - ã¬ã¸ã¹ããªã¨ãã£ã¿ã§ SystemPropertyHandlers ãç·¨éããå¿è¦ããã
- `PropDumper`
    - åå¼·ç®çã§ MS ã®ãµã³ãã«ãåå®è£ãããã¤
        - https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/propertyschemas
        - âããregisteræ©è½ããªããããã®
- `PropSchemaRegister`
    - propdesc ãç»é²ããããã®ãã¤
    - PropDumper ã®ãµã³ãã«ã® register æ©è½ãæç²ããã ã
- `SmartSizePropertyHandler`
    - PropertyHandler ã®å®è£
    - åºæ¬çã« MS ã®ãµã³ãã«ãçºããªããå®è£
        - https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appshellintegration/RecipePropertyHandler


## Note
### åæç¥è­
- PropertyHandler
    - ãã¡ã¤ã«ãããã­ããã£ãåãåºã
    - PropertyHandler ã¯ãåæ¡å¼µå­ã«ã¤ãï¼åããç»é²ã§ããªããæ²ããã
- propdesc
    - ãã­ããã£ã®è¡¨ç¤ºæ¹æ³ã¾ãããæå®
    - ãããç»é²ããªãã¨è©³ç´°è¡¨ç¤ºã®ã¿ãã¨ãã§ä½¿ããªã
- registry
    - SystemPropertyHandlers
        - key: `ã³ã³ãã¥ã¼ã¿ã¼\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\SystemPropertyHandlers`
        - ç·¨éããã«ã¯ã¢ã¯ã»ã¹è¨±å¯ãå¤æ´ããå¿è¦ããã
    - PropertyHandlers
        - key: `ã³ã³ãã¥ã¼ã¿ã¼\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers`
    - SystemPropertyHandlers ãåªåããã


### PropertyHandler ã®ç»é²æ¹æ³
#### Step 1
- Powershell ãç®¡çèæ¨©éã§å®è¡
- ãã®ãã£ã¬ã¯ããªã¾ã§ç§»å
- PropSchemaRegister.exe ã§ propdesc ãç»é²
    - `> .\PropSchemaRegister.exe r .\SmartSize.propdesc`
    - è©³ç´°è¡¨ç¤ºã®ã¿ãè¡¨ç¤ºã«å¿è¦
- regsvr32 ã§ dll ãç»é²
    - `> regsvr32 .\SmartSizePropertyHandler.dll`
    - ç»é²è§£é¤ã¯ `regsvr32 /u hoge.dll`

#### Step 2
- ã¬ã¸ã¹ããªã¨ãã£ã¿ãèµ·å
- SystemPropertyHandlers ãç·¨é
    - key: `ã³ã³ãã¥ã¼ã¿ã¼\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\SystemPropertyHandlers`
    - å¯¾è±¡ã®æ¡å¼µå­ã®ãã³ãã©ãå¤æ´ãã
        - `{E5E96240-C9EE-11EC-9D64-0242AC120002}` ã«å¤æ´
        - ãã¨ã® PropertyHandler ã®æ©è½ã¯å¤±ãããã®ã§æ³¨æ
            - ãã¨ã® uuid ã«æ»ãã°ãï½

#### ã¡ãªã¿ã«
- dll ãã«ãæã«æå®ããæ¡å¼µå­ãããªãã¦ããæåç»é²ããã°ãï½
    - æ¡å¼µå­ãSystemPropertyHandlers ã«â¦
        - ãã â SystemPropertyHandlers ã«ç»é²ããã¦ããã¤ãç·¨é
        - ãªã â PropertyHandlers ã«è¿½å 
            - `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers`

##### ã­ã¼ãã
- SmartSize PropertyHandler
    - key: `{E5E96240-C9EE-11EC-9D64-0242AC120002}`
- Default SystemPropertyHandler
    - mp4: `{f81b1b56-7613-4ee4-bc05-1fab5de5c07e}`
    - mkv: `{C591F150-4106-4141-B5C1-30B2101453BD}`

