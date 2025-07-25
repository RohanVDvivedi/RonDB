#include<rondb/rondb.h>
#include<rondb/transaction_table.h>

#include<stdlib.h>

int main()
{
	rondb rdb;
	initialize_rondb(&rdb, "./test.db",
		5,
		512, 8, 10, 10,
			10000ULL, 100000ULL,
			10000000ULL,
		4096,
			10000000ULL);

	uint64_t root_page_id = 0;

	transaction_table ttbl;
	initialize_transaction_table(&ttbl, &root_page_id, &(rdb.persistent_acid_rage_engine), 3);

	mvcc_snapshot* t1 = get_new_transaction_id(&ttbl);
	print_mvcc_snapshot(t1);

	mvcc_snapshot* t2 = get_new_transaction_id(&ttbl);
	print_mvcc_snapshot(t2);

	mvcc_snapshot* t3 = get_new_transaction_id(&ttbl);
	print_mvcc_snapshot(t3);

	update_transaction_status(&ttbl, t2->transaction_id, TX_ABORTED);

	mvcc_snapshot* t4 = get_new_transaction_id(&ttbl);
	print_mvcc_snapshot(t4);

	update_transaction_status(&ttbl, t3->transaction_id, TX_COMMITTED);

	mvcc_snapshot* t5 = get_new_transaction_id(&ttbl);
	print_mvcc_snapshot(t5);

	deinitialize_mvcc_snapshot(t5);
	free(t5);

	deinitialize_mvcc_snapshot(t4);
	free(t4);

	deinitialize_mvcc_snapshot(t3);
	free(t3);

	deinitialize_mvcc_snapshot(t2);
	free(t2);

	deinitialize_mvcc_snapshot(t1);
	free(t1);

	for(uint256 tid = get_0_uint256(); tid.limbs[0] < 5; add_uint256(&tid, tid, get_1_uint256()))
	{
		transaction_status status = get_transaction_status(&ttbl, tid);
		printf("%"PRIu64" -> %s\n", tid.limbs[0], transaction_status_string[status]);
	}

	deinitialize_rondb(&rdb);

	return 0;
}