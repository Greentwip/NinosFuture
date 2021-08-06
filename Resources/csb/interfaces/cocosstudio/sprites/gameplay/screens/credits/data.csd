<GameFile>
  <PropertyGroup Name="data" Type="Scene" ID="12f10f11-4be7-4c0b-929b-3a833fafe88b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="1200" Speed="1.0000" ActivedAnimationName="scroll">
        <Timeline ActionTag="1511095628" Property="Position">
          <PointFrame FrameIndex="0" X="128.0000" Y="-655.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="1200" X="128.0000" Y="680.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="1511095628" Property="Scale">
          <ScaleFrame FrameIndex="0" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="1200" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1511095628" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="1200" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="scroll" StartIndex="0" EndIndex="1200">
          <RenderColor A="255" R="255" G="105" B="180" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="54" ctype="GameNodeObjectData">
        <Size X="256.0000" Y="224.0000" />
        <Children>
          <AbstractNodeData Name="sides" ActionTag="-1243024821" Tag="97" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/credits/sides-transparent.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="credits" ActionTag="1511095628" Tag="56" IconVisible="False" LeftMargin="20.5000" RightMargin="20.5000" TopMargin="284.0000" BottomMargin="-1250.0000" FontSize="14" LabelText="CREDITS&#xA;&#xA;&#xA;&#xA;Graphics&#xA;&#xA;Valeria Carreon&#xA;&#xA;&#xA;&#xA;Programming&#xA;&#xA;Victor Lopez&#xA;&#xA;&#xA;&#xA;AUDIO&#xA;&#xA;Giorgio Zangarini&#xA;&#xA;Alexander Gruend&#xA;&#xA;&#xA;&#xA;Special Thanks&#xA;&#xA;&#xA;&#xA;Microsoft&#xA;&#xA;&#xA;Cocos2d-x &#xA;&#xA;Developers&#xA;&#xA;&#xA;SDL2 &#xA;&#xA;Developers&#xA;&#xA;&#xA;Solarus&#xA;&#xA;Developers&#xA;&#xA;&#xA;Folks at&#xA;&#xA;Gameloft&#xA;&#xA;&#xA;Ted John&#xA;&#xA;Oclero&#xA;&#xA;Max Mraz&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;&#xA;Thanks for &#xA;&#xA;playing!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="215.0000" Y="1190.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="-655.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="-2.9241" />
            <PreSize X="0.8398" Y="5.3125" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="top" ActionTag="-1260457081" Tag="98" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/credits/top.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>