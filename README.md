
# master_work���

master_work��һ����C�����ģ�������ٴ��˷����ϵͳ�������ģ�ͣ�����̵��߳�ģ�ͣ��ǲ���nginx����ģ�һ���ִ����Ǵ�nginx�������ģ�


## ���ʹ�ã�

## ���master_work��
### 1. ����ģ��
### 2. �¼�����
### 3. recv buf
### 4. send buf
### 5. �ڴ��

## �����䣺


## ��Ҫ���ܣ�

### 1. ��������

- ֧��SQL��д���롣
- ֧��͸����MySQL���ӳأ�����ÿ���½����ӡ�
- ֧��ƽ������DB������DB��ǰ��Ӧ���޸�֪��
- ֧�ֶ��slave��slave֮��ͨ��Ȩֵ���и��ؾ��⡣
- ֧��ǿ�ƶ����⡣
- ֧���������ԣ�java,php,python,C/C++,Go)SDK��mysql��prepare���ԡ�
- ֧�ֵ����DB��������������ơ�
- ֧��SQL��־������־�����
- ֧��SQL���������ơ�
- ֧�ֿͻ���IP���ʰ��������ƣ�ֻ�а������е�IP���ܷ���kingshard��
- ֧���ַ������á�
- ֧��last_insert_id���ܡ�
- ֧�ֶ�̬�޸�kingshard�������������ο�����������
- ֧����Web API���õķ�ʽ����kingshard��

### 2. sharding����

- ֧�ְ�������hash��range�ֱ�ʽ��
- ֧�ְ��ꡢ�¡���ά�ȵ�ʱ��ֱ�ʽ��
- ֧�ֿ�ڵ�ֱ��ӱ���Էֲ��ڲ�ͬ�Ľڵ㡣
- ֧�ֿ�ڵ��count,sum,max��min�ȾۺϺ�����
- ֧�ֵ����ֱ��join��������֧�ֱַ����һ�Ų��ֱ��join������
- ֧�ֿ�ڵ��order by,group by,limit�Ȳ�����
- ֧�ֽ�sql���͵��ض��Ľڵ�ִ�С�
- ֧���ڵ����ڵ���ִ�����񣬲�֧�ֿ��ڵ�ķֲ�ʽ����
- ֧�ַ�����ʽ���£�insert,delete,update,replace�����node�ϵ��ӱ�

## kinshard�ĵ�

### kingshard��װ��ʹ��

[1.��װkingshard](./doc/KingDoc/kingshard_install_document.md)

[2.�������һ�����ݿ��м����չMySQL��Ⱥ����kingshardʹ��ָ��](./doc/KingDoc/how_to_use_kingshard.md)

[3.kingshard sharding����](./doc/KingDoc/kingshard_sharding_introduce.md)

[4.kingshard��ʱ��ֱ��ܽ���](./doc/KingDoc/kingshard_date_sharding.md)

[5.kingshard ��������](./doc/KingDoc/kingshard_quick_try.md)

[6.������������](./doc/KingDoc/admin_command_introduce.md)

[7.�����Web API�ӿڽ���](./doc/KingDoc/kingshard_admin_api.md)

[8.kingshard SQL���������ܽ���](./doc/KingDoc/sql_blacklist_introduce.md)

[9.kingshard��FAQ](./doc/KingDoc/function_FAQ.md)

[10.kingshard SQL֧�ַ�Χ](./doc/KingDoc/kingshard_support_sql.md)

[11.������LVSʵ�ּ�Ⱥ����](./doc/KingDoc/how_to_use_lvs.md)

### kingshard�ܹ������

[1.kingshard�ܹ���ƺ͹���ʵ��](./doc/KingDoc/architecture_of_kingshard_CN.md)

[2.kingshard�����Ż�֮����ƪ](./doc/KingDoc/kingshard_performance_profiling.md)

[3.kingshard���ܲ��Ա���](./doc/KingDoc/kingshard_performance_test.md)
## ��л
- ��л[mixer](https://github.com/siddontang/mixer)����siddontang, kingshard����İ汾���ǻ���mixer���������ġ�
- ��л[bigpyer](https://github.com/bigpyer)������kingshard������ϸ�����ܲ��ԣ���׫д��һ�ݷǳ���ϸ�Ĳ��Ա��档
- ��л����[��Դ������](https://github.com/flike/kingshard/graphs/contributors)Ϊkingshard�����Ĺ��ס�

## kingshard�û��б�

https://github.com/flike/kingshard/issues/148

## ����
kingshard��Դ�������������ϵص������������ܽ�Ϊ���ƣ��ȶ����нϴ������� **Ŀǰ�����϶�ʮ�ҹ�˾����������ʹ��kingshard��ΪMySQL����** �������ʹ��kingshard�Ĺ����з���BUG�������µĹ��������뷢�ʼ���flikecn#126.com������ȡ����ϵ�����߼���QQȺ(147926796)������
��ӭ��ע**��˼�����Ѷ**���ںţ��й�kingshard��������Ϣ���˼ܹ����������£�������������ںŷ���

<img src="./doc/KingDoc/wechat_pic.png" width="20%" height="20%">

## License

kingshard����Apache 2.0Э�飬���Э����ο�[Ŀ¼](./doc/License)