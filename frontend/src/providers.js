/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

const baseURL = ( () => {
	if ( window.location.origin === 'null' || window.location.origin === 'file://' )
		return 'http://127.0.0.1:3001/api';
	else
		return window.location.pathname + 'api';
})();

const httpClient = axios.create({
	baseURL: baseURL,
	headers: { 
		'Content-Type': 'application/json',
		'Accept-Language': window.navigator.language,
	},
})

httpClient.interceptors.response.use( response => {
	return response;
}, error => {
	console.log(error);
	if ( !error.response ) {
		error.response = { data: { 'message': error.message } };
	} else if ( !error.response.data ) {
		error.response.data = { 'message': error.message };
	} else if ( !error.response.data.message ) {
		error.response.data = { 'message': error.response.statusText };
	}
	return Promise.reject( error );
})

const dataProvider = {
	namespaced: true,
	state: () => ({
		loading: false,
		errors: {},
		data: {},
	}),
	mutations: {
		setLoading: (state, loading) => state.loading = loading,
		setErrors: (state, errors ) => state.errors = errors,
		setData: (state, data ) => state.data = data,
	},
	actions: {
		getList: async( { state, commit }, params ) => {
			commit( 'setLoading', true );
			await httpClient.get( `${state._namespace}`, { params: params })
			.then( response => {
				commit( 'setData', response.data );
			})
			.catch( error => {
				if ( error.response.data.errors ) {
					commit('setErrors', error.response.data.errors )
				}
				commit( 'showToast', 
					{ color: 'red', message: error.response.data.message }, 
					{ root: true } )
			})
			.finally( () => commit( 'setLoading', false ) );
		},
		create: async({ state, commit }, params) => {
			commit('setLoading', true );
			await httpClient.post( `${state._namespace}`, params )
			.then( response => {
				commit( 'setData', response.data );
			})
			.catch( error => {
				if ( error.response.data.errors ) {
					commit('setErrors', error.response.data.errors )
				}
				commit( 'showToast', 
					{ color: 'red', message: error.response.data.message }, 
					{ root: true } )
			})
			.finally( () => commit( 'setLoading', false ) );
		},
	},
};
