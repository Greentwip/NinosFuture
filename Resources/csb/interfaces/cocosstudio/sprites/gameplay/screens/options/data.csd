<GameFile>
  <PropertyGroup Name="data" Type="Scene" ID="a2ee0952-26b5-49ae-8bf9-4f1d6279b798" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" ctype="GameNodeObjectData">
        <Size X="256.0000" Y="224.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="573775979" Tag="19" IconVisible="True" RightMargin="256.0000" TopMargin="224.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="image" ActionTag="-2042783985" Tag="3" IconVisible="False" RightMargin="-256.0000" TopMargin="-224.0000" ctype="SpriteObjectData">
                <Size X="256.0000" Y="224.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.0000" Y="112.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/options/background.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="border" ActionTag="1582786742" Tag="4" IconVisible="False" RightMargin="-256.0000" TopMargin="-224.0000" ctype="SpriteObjectData">
                <Size X="256.0000" Y="224.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.0000" Y="112.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/options/border.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="-1782551435" Tag="21" IconVisible="False" LeftMargin="37.0000" RightMargin="-91.0000" TopMargin="-204.0000" BottomMargin="196.0000" FontSize="8" LabelText="OPTIONS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="54.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="64.0000" Y="200.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bgm" ActionTag="754586535" Tag="5" IconVisible="True" LeftMargin="144.0000" RightMargin="112.0000" TopMargin="89.0000" BottomMargin="135.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="tab" ActionTag="691227743" Tag="6" IconVisible="False" LeftMargin="-60.0000" RightMargin="-60.0000" TopMargin="-17.5000" BottomMargin="-17.5000" ctype="SpriteObjectData">
                <Size X="120.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/options/tab.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="1887939368" Tag="7" IconVisible="False" LeftMargin="-38.5000" RightMargin="-38.5000" TopMargin="-12.0000" BottomMargin="4.0000" FontSize="8" LabelText="BGM VOLUME" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="77.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position Y="8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="slider" ActionTag="1869557533" Tag="8" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="3.0000" BottomMargin="-13.0000" TouchEnable="True" PercentInfo="50" ctype="SliderObjectData">
                <Size X="92.0000" Y="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position Y="-8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <BackGroundData Type="Default" Path="Default/Slider_Back.png" Plist="" />
                <ProgressBarData Type="Default" Path="Default/Slider_PressBar.png" Plist="" />
                <BallNormalData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
                <BallPressedData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
                <BallDisabledData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="144.0000" Y="135.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5625" Y="0.6027" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="sfx" ActionTag="81419048" Tag="11" IconVisible="True" LeftMargin="144.0000" RightMargin="112.0000" TopMargin="144.0000" BottomMargin="80.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="tab" ActionTag="2130449621" Tag="12" IconVisible="False" LeftMargin="-60.0000" RightMargin="-60.0000" TopMargin="-17.5000" BottomMargin="-17.5000" ctype="SpriteObjectData">
                <Size X="120.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/options/tab.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="587603206" Tag="13" IconVisible="False" LeftMargin="-38.0000" RightMargin="-38.0000" TopMargin="-12.0000" BottomMargin="4.0000" FontSize="8" LabelText="SFX VOLUME" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="76.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position Y="8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="slider" ActionTag="185201647" Tag="14" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="3.0000" BottomMargin="-13.0000" TouchEnable="True" PercentInfo="49" ctype="SliderObjectData">
                <Size X="92.0000" Y="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position Y="-8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <BackGroundData Type="Default" Path="Default/Slider_Back.png" Plist="" />
                <ProgressBarData Type="Default" Path="Default/Slider_PressBar.png" Plist="" />
                <BallNormalData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
                <BallPressedData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
                <BallDisabledData Type="Normal" Path="sprites/gameplay/screens/options/button.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="144.0000" Y="80.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="127" G="127" B="127" />
            <PrePosition X="0.5625" Y="0.3571" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>