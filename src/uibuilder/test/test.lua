
local panel1 = createPanel("Panel1")

createPanel("Panel2")

local panel3 = createPanel("Panel3")

addButton(panel1, "A button")

for i=1,10 do
    addButton(panel3, "Button"..i)
end