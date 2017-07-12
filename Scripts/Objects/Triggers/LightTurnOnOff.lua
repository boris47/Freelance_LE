

function Script:Start()
	self.on = not self.entity:Hidden();
end

function Script:Toggle()--in
	self.on = not self.on;
	if ( self.on ) then
		self.entity:Show();
	else
		self.entity:Hide();
	end
end