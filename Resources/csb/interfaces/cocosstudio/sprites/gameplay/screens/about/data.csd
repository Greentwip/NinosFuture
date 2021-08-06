<GameFile>
  <PropertyGroup Name="data" Type="Scene" ID="9cb41fcf-cc25-4bcd-8561-cafa02944433" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="39" ctype="GameNodeObjectData">
        <Size X="256.0000" Y="224.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="502637089" Tag="71" IconVisible="False" ctype="SpriteObjectData">
            <Size X="256.0000" Y="224.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="128.0000" Y="112.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="sprites/gameplay/screens/about/background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="next_button" ActionTag="478713255" Tag="62" IconVisible="True" LeftMargin="178.0000" RightMargin="78.0000" TopMargin="192.0000" BottomMargin="32.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="button" ActionTag="1288925769" Tag="63" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="-12.0000" BottomMargin="-12.0000" ctype="SpriteObjectData">
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
              <AbstractNodeData Name="label" ActionTag="-854724568" Tag="64" IconVisible="False" LeftMargin="-23.5000" RightMargin="-23.5000" TopMargin="-6.0000" BottomMargin="-6.0000" FontSize="12" LabelText="NEXT" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="47.0000" Y="12.0000" />
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
          <AbstractNodeData Name="prev_button" ActionTag="573930493" Tag="65" IconVisible="True" LeftMargin="78.0000" RightMargin="178.0000" TopMargin="192.0000" BottomMargin="32.0000" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <Children>
              <AbstractNodeData Name="button" ActionTag="59484437" Tag="66" IconVisible="False" LeftMargin="-46.0000" RightMargin="-46.0000" TopMargin="-12.0000" BottomMargin="-12.0000" ctype="SpriteObjectData">
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
              <AbstractNodeData Name="label" ActionTag="-2012906389" Tag="67" IconVisible="False" LeftMargin="-25.0000" RightMargin="-25.0000" TopMargin="-6.0000" BottomMargin="-6.0000" FontSize="12" LabelText="PREV" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="50.0000" Y="12.0000" />
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
          <AbstractNodeData Name="heading" ActionTag="-538843030" Tag="68" IconVisible="False" LeftMargin="107.1490" RightMargin="107.8510" TopMargin="20.0000" BottomMargin="188.0000" FontSize="8" LabelText="About&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="41.0000" Y="16.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="127.6490" Y="196.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4986" Y="0.8750" />
            <PreSize X="0.1602" Y="0.0714" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_0" Visible="False" ActionTag="-803803686" VisibleForFrame="False" Tag="33" IconVisible="False" LeftMargin="32.0000" RightMargin="22.0000" TopMargin="56.0000" BottomMargin="80.0000" FontSize="8" LabelText="Greentwip is an open-source&#xA;&#xA;&#xA;Initiative that hopes to bring&#xA;&#xA;&#xA;the classic video games to &#xA;&#xA;&#xA;the current generation&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="202.0000" Y="88.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7891" Y="0.3929" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_1" Visible="False" ActionTag="2031561642" VisibleForFrame="False" Tag="34" IconVisible="False" LeftMargin="32.0000" RightMargin="27.0000" TopMargin="56.0000" BottomMargin="64.0000" FontSize="8" LabelText="We've received support from&#xA;&#xA;&#xA;Microsoft in licensing and&#xA;&#xA;&#xA;development equipment &#xA;&#xA;&#xA;and looks like our&#xA;&#xA;&#xA;goal slowly accomplishes" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="197.0000" Y="104.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7695" Y="0.4643" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_2" Visible="False" ActionTag="179572334" VisibleForFrame="False" Tag="35" IconVisible="False" LeftMargin="32.0000" RightMargin="28.0000" TopMargin="56.0000" BottomMargin="72.0000" FontSize="8" LabelText="On the creative side&#xA;&#xA;Regan and Nino are part&#xA;&#xA;of a fantasy world governed&#xA;&#xA;by tech pirates, inspired by &#xA;&#xA;the wars of the conquest &#xA;&#xA;ages&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="196.0000" Y="96.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7656" Y="0.4286" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_3" Visible="False" ActionTag="398174653" VisibleForFrame="False" Tag="36" IconVisible="False" LeftMargin="32.0000" RightMargin="23.0000" TopMargin="56.0000" BottomMargin="56.0000" FontSize="8" LabelText="Nino's Future takes place &#xA;&#xA;3000 years after Nino's &#xA;&#xA;teenager life, and it's Swing, &#xA;&#xA;one of the human survivors &#xA;&#xA;who fight against robotic &#xA;&#xA;androids taking over the &#xA;&#xA;world&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="201.0000" Y="112.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7852" Y="0.5000" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_4" Visible="False" ActionTag="-1322635878" VisibleForFrame="False" Tag="37" IconVisible="False" LeftMargin="32.0000" RightMargin="36.0000" TopMargin="56.0000" BottomMargin="80.0000" FontSize="8" LabelText="We're open to ideas and &#xA;&#xA;contributions from all the&#xA;&#xA;world and invite you to join&#xA;&#xA;us into this adventure&#xA;&#xA;&#xA;&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="188.0000" Y="88.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7344" Y="0.3929" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_5" ActionTag="1359320616" VisibleForFrame="False" Tag="38" IconVisible="False" LeftMargin="32.0000" RightMargin="21.0000" TopMargin="56.0000" BottomMargin="80.0000" FontSize="8" LabelText="You can reach us by email&#xA;&#xA;development at greentwip.xyz&#xA;&#xA;find us on facebook as&#xA;&#xA;greentwip in Spanish&#xA;&#xA;and Twiiter greentwiphq&#xA;&#xA;in English" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="203.0000" Y="88.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7930" Y="0.3929" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_6" Visible="False" ActionTag="-1393013062" VisibleForFrame="False" Tag="39" IconVisible="False" LeftMargin="32.0000" RightMargin="30.0000" TopMargin="56.0000" BottomMargin="88.0000" FontSize="8" LabelText="This is a beta version&#xA;&#xA;which means might be bugs&#xA;&#xA;we're working hard to &#xA;&#xA;deliver the full version&#xA;&#xA;which will come as an update&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="194.0000" Y="80.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.7578" Y="0.3571" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_7" Visible="False" ActionTag="1105256367" VisibleForFrame="False" Tag="40" IconVisible="False" LeftMargin="32.0000" RightMargin="48.0000" TopMargin="56.0000" BottomMargin="96.0000" FontSize="8" LabelText="A reasonable part of the&#xA;&#xA;earnings will go directly&#xA;&#xA;to gaming Open Source &#xA;&#xA;Software such as &#xA;&#xA;Solarus and Pygame" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="176.0000" Y="72.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="32.0000" Y="168.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1250" Y="0.7500" />
            <PreSize X="0.6875" Y="0.3214" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="about_8" Visible="False" ActionTag="-1682701482" VisibleForFrame="False" Tag="41" IconVisible="False" LeftMargin="48.0000" RightMargin="44.0000" TopMargin="76.0000" BottomMargin="116.0000" FontSize="8" LabelText="We hope you have fun&#xA;&#xA;and thanks for playing!&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
            <Size X="164.0000" Y="32.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position X="48.0000" Y="148.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1875" Y="0.6607" />
            <PreSize X="0.6406" Y="0.1429" />
            <FontResource Type="Normal" Path="megaman_2.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>