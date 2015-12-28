-- this is a comment

local function factorial(n)
    local fact = 1;

    while n > 0 do
    	  fact = fact * n
	  n = n - 1
    end
 
    return fact
end

print (factorial(5))