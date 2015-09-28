#ifndef _CRYPTO_H
#define _CRYPTO_H
#include  <stdint.h>
#include  <stdbool.h>


typedef enum{
    CRYPTO_OK,		//< Все добре
    CRYPTO_BAD_ARG, //< Поганий аргумент
    CRYPTO_NO_MEM,	//< Недостатньо пам'яті
    CRYPTO_NO_KEY,	//< Ключ шифрування не встановлено
    CRYPTO_NO_IV	//< Вектор ініціалізації(синхропосилку) не встановлено
}tcrypto_error;


typedef tcrypto_error(*tcrypto_f_init)(void **, void*); 									//< Тип функції ініціалізації  приватної структури
typedef tcrypto_error(*tcrypto_f_end)(void *);												//< Тип функції очистки пам'яті виділеної для приватної стрктури 
typedef tcrypto_error(*tcrypto_ed)(void *, void *, void *, const uint32_t, const bool);		//< Тип функцій шифрування та розшифрування
typedef tcrypto_error(*tcrypto_set)(void *, void *);										//< Тип функції встановлення синхропосилки і ключа

// Назви функцій
#define CRYPTO_INIT_FUNC "crypto_init"
#define CRYPTO_FREE_FUNC "crypto_end"
#define CRYPTO_ENCRYPT_FUNC "crypto_encrypt"
#define CRYPTO_DECRYPT_FUNC "crypto_decrypt"
#define CRYPTO_SET_KEY_FUNC "crypto_set_key"
#define CRYPTO_SET_IV_FUNC "crypto_set_iv"

/**
 * Виконує початкову ініціалізацію приватної стрктури
 * @param handler Вказівник на вказівник на приватну структуру данних
 * @param s Вказівник на таблицю замін(s-блоків)
 * @return CRYPTO_OK, CRYPTO_BAD_ARG
 */
tcrypto_error crypto_init        (void **handler,    void *s);

/**
 * Виконує установку ключа для шифрування
 * @param handler Вказівник на приватну структуру данних
 * @param k Вказівник на ключ
 * @return CRYPTO_OK, CRYPTO_BAD_ARG
 */
tcrypto_error crypto_set_key     (void *handler,     void *k);

/**
 * Виконує установку синхропосилки для шифрування
 * Перед визовом данної функції повинно бути встановлено ключ
 * @param handler Вказівник на приватну структуру данних
 * @param i Вказівник на синхропосилку
 * @return CRYPTO_OK, CRYPTO_BAD_ARG, CRYPTO_NO_KEY
 */
tcrypto_error crypto_set_iv      (void *handler,     void *i);

/**
 * Виконує шифрування блоку данных
 * Перед визовом данної функції повинно бути встановлено ключ і синхропосилку
 * @param handler Вказівник на приватну структуру данних
 * @param out  Вказівник на вихідний буфер
 * @param in   Вказівник на вхідний буфер
 * @param size Розмір буферу
 * @param last Чи це останій буфер данних
 * @return CRYPTO_OK, CRYPTO_NO_KEY, CRYPTO_NO_IV
 */
tcrypto_error crypto_encrypt     (void *handler,     void *out, void *in, const uint32_t size, const bool last);

/**
 * Виконує розшифрування блоку данных
 * Перед визовом данної функції повинно бути встановлено ключ і синхропосилку
 * @param handler Вказівник на приватну структуру данних
 * @param out  Вказівник на вихідний буфер
 * @param in   Вказівник на вхідний буфер
 * @param size Розмір буферу
 * @param last Чи це останій буфер данних
 * @return CRYPTO_OK, CRYPTO_NO_KEY, CRYPTO_NO_IV
 */
tcrypto_error crypto_decrypt     (void *handler,     void *out, void *in, const uint32_t size, const bool last);

/**
 * Виконує очистку пам'яті виділеної для приватної структури
 * @param handler Вказівник на приватну структуру данних
 */
void          crypto_end         (void *handler);


#endif //_CRYPTO_H
