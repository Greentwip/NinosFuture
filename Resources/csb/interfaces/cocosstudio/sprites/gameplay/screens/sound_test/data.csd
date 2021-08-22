<GameFile>
  <PropertyGroup Name="data" Type="Scene" ID="cec66169-dbf5-48b2-87e5-57512e1cfbd7" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="34" ctype="GameNodeObjectData">
        <Size X="256.0000" Y="224.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="1295380175" Tag="35" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/sound_test/background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="border" ActionTag="8344706" Tag="36" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/sound_test/border.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="play_button" ActionTag="-626850934" Tag="38" IconVisible="False" LeftMargin="48.0000" RightMargin="176.0000" TopMargin="184.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="14" ButtonText="Play" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="64.0000" Y="24.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.2500" Y="0.1071" />
            <PreSize X="0.1250" Y="0.1429" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="stop_button" ActionTag="-107356131" Tag="39" IconVisible="False" LeftMargin="196.0000" RightMargin="28.0000" TopMargin="184.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="14" ButtonText="Stop" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="212.0000" Y="24.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.8281" Y="0.1071" />
            <PreSize X="0.1250" Y="0.1429" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="slider" ActionTag="154271496" Tag="40" IconVisible="False" LeftMargin="40.0000" RightMargin="16.0000" TopMargin="113.7621" BottomMargin="96.2379" TouchEnable="True" PercentInfo="50" ctype="SliderObjectData">
            <Size X="200.0000" Y="14.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="140.0000" Y="103.2379" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5469" Y="0.4609" />
            <PreSize X="0.7813" Y="0.0625" />
            <BackGroundData Type="Default" Path="Default/Slider_Back.png" Plist="" />
            <ProgressBarData Type="Default" Path="Default/Slider_PressBar.png" Plist="" />
            <BallNormalData Type="Default" Path="Default/SliderNode_Normal.png" Plist="" />
            <BallPressedData Type="Default" Path="Default/SliderNode_Press.png" Plist="" />
            <BallDisabledData Type="Default" Path="Default/SliderNode_Disable.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="title" ActionTag="-2021288696" Tag="41" IconVisible="False" LeftMargin="65.0000" RightMargin="41.0000" TopMargin="48.0000" BottomMargin="164.0000" FontSize="12" LabelText="Current Sound" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="150.0000" Y="12.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="140.0000" Y="170.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5469" Y="0.7589" />
            <PreSize X="0.5859" Y="0.0536" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="current_sound" ActionTag="-657663846" Tag="42" IconVisible="False" LeftMargin="123.0000" RightMargin="99.0000" TopMargin="88.0000" BottomMargin="128.0000" FontSize="8" LabelText="None" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="34.0000" Y="8.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="140.0000" Y="132.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5469" Y="0.5893" />
            <PreSize X="0.1328" Y="0.0357" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="previous_button" ActionTag="1951703497" Tag="43" IconVisible="False" LeftMargin="96.0000" RightMargin="128.0000" TopMargin="184.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="14" ButtonText="&lt;" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="112.0000" Y="24.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4375" Y="0.1071" />
            <PreSize X="0.1250" Y="0.1429" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="next_button" ActionTag="-1164552002" Tag="44" IconVisible="False" LeftMargin="144.0000" RightMargin="80.0000" TopMargin="184.0000" BottomMargin="8.0000" TouchEnable="True" FontSize="14" ButtonText="&gt;" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="160.0000" Y="24.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6250" Y="0.1071" />
            <PreSize X="0.1250" Y="0.1429" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="amplitude_text" ActionTag="878237993" Tag="45" IconVisible="False" LeftMargin="48.0000" RightMargin="132.0000" TopMargin="148.0000" BottomMargin="68.0000" FontSize="8" LabelText="Amplitude:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="76.0000" Y="8.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="86.0000" Y="72.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.3359" Y="0.3214" />
            <PreSize X="0.2969" Y="0.0357" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="amplitude" ActionTag="-1157138315" Tag="46" IconVisible="False" LeftMargin="140.0000" RightMargin="96.0000" TopMargin="148.0000" BottomMargin="68.0000" FontSize="8" LabelText="1.0" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="20.0000" Y="8.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="150.0000" Y="72.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5859" Y="0.3214" />
            <PreSize X="0.0781" Y="0.0357" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="save_button" ActionTag="249518053" Tag="17" IconVisible="False" LeftMargin="196.0000" RightMargin="28.0000" TopMargin="138.0000" BottomMargin="54.0000" TouchEnable="True" FontSize="14" ButtonText="Save" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="212.0000" Y="70.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.8281" Y="0.3125" />
            <PreSize X="0.1250" Y="0.1429" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>