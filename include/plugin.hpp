#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#ifdef WIN64
	#include <windows.h>
#endif

#define MAX_FUNCTION_COUNT 512

namespace plugin_api{
	/**@brief FunctionType - тип функции 
	  * @details Тип функции может принимать следующие значения:
	  * In - функция принимает аргументы, но ничего не возвращает.
	  * Out - функция не принимает аргументов, но возвращает значение(массив значений)
	  * InOut - функция принимает аргументы и возвращает значение
	  * NotInOut - функция не принимает аргументов и не возвращает значение
	  */
	enum FunctionType{
		NotLoad = 0,
		In,
		Out,
		InOut,
		NotInOut
	};
	
	/**@brief PluginError - Тип ошибки плагина
	  * @details Может принимать следующие значения:
	  * Success - успех
	  * NullArg - функция плагина ожидала аргументы но не получила ни одного
	  * NullRet - переданный функции плагина указатель на переменную возвращаемого значения пуст
	  * MissingArg - пропущен один аргумент функции плагина или более
	  * InvalidArg - недопустимое значение аргумента переданного в функцию плагина
	  * PluginNotExists - Загружаемый плагин не найден в папке $UVA_ROOT/Plagins
	  * PluginNameIsNull - Пустое имя плагина
	  * PluginNotFound - Плагин не найден в загруженных
	  * FunctionNotExists - Функция отсутствует в плагине
	  * FunctionNameNotAllow - Недопустимое имя функции
	  * FunctionRetWithError - Функция завершилась с ошибкой
	  * FunctionCrash - Функция упала с исключением
	  * FunctionCountLimitOverflow - Привышен лимит количества функций на плагин
	  * InvalidFunctionCount - Недопустимое значение количества функций
	  */
	enum PluginError{
		Success,
		NullArg,
		NullRet,
		MissingArg,
		InvalidArg,
		PluginNotExists,
		PluginDllError,
		PluginNameIsNull,
		PluginNotFound,
		FunctionNotExists,
		FunctionNameNotAllow,
		FunctionRetWithError,
		FunctionCrash,
		FunctionCountLimitOverflow,
		InvalidFunctionCount
	};
	extern "C"{
	/** @brief pfarg - аргумент функции плагина
	  * @details Общий тип данных для передачи аргументов плагину
	  */
	typedef struct _pfarg {
		void *argValuePtr;
		_pfarg *argNext;
	} pfarg;
	
	///Тип адреса вызываемой функции
	typedef PluginError (*function_address)(pfarg *ret,pfarg *arg);
	
	/** @brief plugin_function - функция плагина
	  * @details Хранит данные необходимые для вызова функции
	  */
	typedef struct _plugin_function{
		///Имя фуннкции
		char* functionName;
		///Адрес для вызова функции
		function_address functionAddress;
	} plugin_function;
	
	/** @brief plugin - плагин
	  * @details Хранит все экспортированные из плагина функции и ссылку на загруженную динамическую библиотеку плагина
	  * на загруженную динамическую библиотеку плагина
	  */
	typedef struct _plugin{
		///Имя плагина
		char *pluginName;
		#ifdef __linux__
			///Ссылка на динамическую библиотеку плагина
			void *dllLibrary;
		#endif
		#ifdef WIN64
			///Ссылка на динамическую библиотеку плагина
			HMODULE dllLibrary;
		#endif
		///Экспортированные из плагина функции
		plugin_function pluginFunctions[MAX_FUNCTION_COUNT];
		///Количество экспортированных функций
		unsigned short int functionCount;
	} plugin;
	
	typedef char** (*export_function)(unsigned short int *count);
	
	/** @brief is_loaded - проверяет наличие плагина в списке загруженных
	  * @param plugin_name - имя плагина
	  * @return bool(true - плагин загружен,false - не загружен)
	  */
	bool is_loaded(char *plugin_name);
	
	/** @brief load - функция загрузки плагина
	  * @param plugin_name - имя плагина
	  * @return PluginError(Возможные значения Success,PluginNameIsNull,PluginNotExists,FunctionCountLimitOverflow,InvalidFunctionCount,FunctionNotExists,FunctionNameNotAllow)
	  */
	
	PluginError load(char *plugin_name);
	
	/** @brief exec - вызов функции плагина
	  * @param plugin_name - имя плагина
	  * @return PluginError(Возможные значения Success,PluginNameIsNull,PluginNotFound,FunctionNotExists,FunctionRetWithError,FunctionCrash,NullArg,NullRet,MissingArg,InvalidArg)
	  */
	PluginError exec(char *plugin_name,char *function_name,pfarg *ret,pfarg *arg);
	
	/** @brief load - функция выгрузки плагина
	  * @param plugin_name - имя плагина
	  */
	void unload(char *plugin_name);
	}
};
#endif
