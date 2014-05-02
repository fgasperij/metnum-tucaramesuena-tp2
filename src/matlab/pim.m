function [vec,value]=pim(start,A,iter)
x=start;
for i=1:iter
	x=A*x;
	n=norm(x);
	x=x/n;
	i = i+1;
end
vec=x;
value=n;
