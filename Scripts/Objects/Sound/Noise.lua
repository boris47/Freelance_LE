Script.soundfile=""--path "File" "Waveform Audio File Format (*.wav):wav|Sound"
Script.playing=true--bool "Playing"
Script.enabled=true--bool "Enabled"
Script.range=50--float "Range"
Script.volume=100--int "Volume"
Script.pitch=1.0--float "Pitch"
Script.loop=true--bool "Loop"


Script.sSourcePath = "" --string "Source Path"

function Script:Start()
	
	self.source = Engine():AudioDevice():FindSource( self.sSourcePath )
	if ( not self.source ) then
		Debug:Error( "Noise " .. self.entity:GetKeyValue( "name" ) .. "has no valid audio source" );
		return;
	end

--[[
	self.source = Source:Create()
	self.source:SetVolume(self.volume/100)
	self.source:SetPitch(self.pitch)
	self.source:SetLoopMode(self.loop)
	self.source:SetRange(self.range)
	self.source:SetPosition(self.entity:GetPosition(true))
	local sound = Sound:Load(self.soundfile)
	if sound~=nil then
		self.source:SetSound(sound)
		if self.playing==true and self.enabled==true then
			self.source:Play()
		end
		sound:Release()
		sound=nil
	end
]]
end

function Script:Play()--in

	if self.enabled and self.source then
		self.source:Play3D( self.entity:GetPosition( true ), self.loop )
		:SetVolume( self.volume / 100 ):SetPitch( self.pitch );
--		self.source:Play()
		print"Play"
	end
	self.component:CallOutputs("Play")
end

function Script:Enable()--in
	self.enabled=true
end

function Script:Disable()--in
	self.enabled=false
end

function Script:Pause()--in
	self.source:Pause()
end
--[[
function Script:Release()
	if self.source then
		self.source:Release()
		self.source=nil
	end
end
]]