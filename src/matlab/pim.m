function [vec,value]=pim(start,A,iter)
v=start;
for i=1:iter
	v=A*v;
	n=norm(v);
	v=v/n;
end
vec=v;
value=(v'*A*v)/(v'*v);
