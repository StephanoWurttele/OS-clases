volatile int j;
int k;

int main(){
  register int i;
  for ( i = 20; i < 200; ++i);
  for ( j = 40; i < 200; ++i);
  for ( k = 40; i < 200; ++i);
}
