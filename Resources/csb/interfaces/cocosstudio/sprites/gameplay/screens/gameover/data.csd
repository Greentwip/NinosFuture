<GameFile>
  <PropertyGroup Name="data" Type="Scene" ID="8a60d564-62e1-4dea-ad77-860844140ae6" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="135" Speed="1.0000" ActivedAnimationName="fly_in_secondary">
        <Timeline ActionTag="456892477" Property="Position">
          <PointFrame FrameIndex="30" X="128.0000" Y="36.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="40" X="128.0000" Y="36.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="70" X="128.0000" Y="-76.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="456892477" Property="Scale">
          <ScaleFrame FrameIndex="40" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="456892477" Property="RotationSkew">
          <ScaleFrame FrameIndex="40" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="70" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="2055334927" Property="Position">
          <PointFrame FrameIndex="90" X="128.0000" Y="-76.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="135" X="128.0000" Y="128.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="2055334927" Property="Scale">
          <ScaleFrame FrameIndex="90" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="135" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="2055334927" Property="RotationSkew">
          <ScaleFrame FrameIndex="90" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="135" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="fly_out_primary" StartIndex="40" EndIndex="70">
          <RenderColor A="255" R="250" G="250" B="210" />
        </AnimationInfo>
        <AnimationInfo Name="fly_in_primary" StartIndex="0" EndIndex="30">
          <RenderColor A="255" R="102" G="205" B="170" />
        </AnimationInfo>
        <AnimationInfo Name="fly_in_secondary" StartIndex="90" EndIndex="135">
          <RenderColor A="255" R="169" G="169" B="169" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="102" ctype="GameNodeObjectData">
        <Size X="256.0000" Y="224.0000" />
        <Children>
          <AbstractNodeData Name="background_1" ActionTag="-809501828" Tag="103" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="yes_button" ActionTag="1864395903" Tag="13" IconVisible="True" LeftMargin="78.0000" RightMargin="178.0000" TopMargin="192.0000" BottomMargin="32.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="button" ActionTag="2086535796" Tag="17" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="-12.0000" BottomMargin="-12.0000" ctype="SpriteObjectData">
                <Size X="92.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/button_left_active.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="-1924962386" Tag="19" IconVisible="False" LeftMargin="-23.5000" RightMargin="-23.5000" TopMargin="-8.0000" BottomMargin="-8.0000" FontSize="16" LabelText="YES" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="47.0000" Y="16.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
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
            <Position X="78.0000" Y="32.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.3047" Y="0.1429" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="no_button" ActionTag="369342008" Tag="16" IconVisible="True" LeftMargin="178.0000" RightMargin="78.0000" TopMargin="192.0000" BottomMargin="32.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="button" ActionTag="-1502068946" Tag="18" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="-12.0000" BottomMargin="-12.0000" ctype="SpriteObjectData">
                <Size X="92.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/button_right_active.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="1552724229" Tag="20" IconVisible="False" LeftMargin="-16.5000" RightMargin="-16.5000" TopMargin="-8.0000" BottomMargin="-8.0000" FontSize="16" LabelText="NO" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="33.0000" Y="16.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
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
            <Position X="178.0000" Y="32.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6953" Y="0.1429" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="game_over_background" ActionTag="863818223" Tag="16" IconVisible="False" LeftMargin="39.0000" RightMargin="39.0000" TopMargin="44.0000" BottomMargin="76.0000" ctype="SpriteObjectData">
            <Size X="178.0000" Y="104.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="128.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5714" />
            <PreSize X="0.6953" Y="0.4643" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/game_over_background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="level_clear_background" ActionTag="-603581630" Tag="17" IconVisible="False" LeftMargin="39.0000" RightMargin="39.0000" TopMargin="44.0000" BottomMargin="76.0000" ctype="SpriteObjectData">
            <Size X="178.0000" Y="104.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="128.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5714" />
            <PreSize X="0.6953" Y="0.4643" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/level_clear_background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="fly_primary_window" ActionTag="456892477" Tag="107" IconVisible="True" LeftMargin="128.0000" RightMargin="128.0000" TopMargin="300.0000" BottomMargin="-76.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="text_window" ActionTag="1500468309" Tag="105" IconVisible="False" LeftMargin="-104.0000" RightMargin="-104.0000" TopMargin="-27.5000" BottomMargin="-27.5000" ctype="SpriteObjectData">
                <Size X="208.0000" Y="55.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/text_window.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="-2007268087" Tag="106" IconVisible="False" LeftMargin="-58.5000" RightMargin="-58.5000" TopMargin="-8.0000" BottomMargin="-8.0000" FontSize="16" LabelText="FINISHED" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="117.0000" Y="16.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
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
            <Position X="128.0000" Y="-76.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="-0.3393" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="fly_secondary_window" ActionTag="2055334927" Tag="10" IconVisible="True" LeftMargin="128.0000" RightMargin="128.0000" TopMargin="96.0000" BottomMargin="128.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="floating_window" ActionTag="-1110905251" Tag="11" IconVisible="False" LeftMargin="-52.5000" RightMargin="-52.5000" TopMargin="-14.0000" BottomMargin="-14.0000" ctype="SpriteObjectData">
                <Size X="105.0000" Y="28.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="sprites/gameplay/screens/gameover/floating_window.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="label" ActionTag="-1952149136" Tag="12" IconVisible="False" LeftMargin="-43.5000" RightMargin="-43.5000" TopMargin="-5.0000" BottomMargin="-5.0000" FontSize="10" LabelText="CONTINUE?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="87.0000" Y="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position />
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
            <Position X="128.0000" Y="128.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5714" />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>