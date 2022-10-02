/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

Vue.use(VueRouter)
Vue.use(VueI18n)
Vue.use(Vuetify)
Vue.use(Vuex)

const i18n = new VueI18n({
	//messages: { 'en': en, 'ru', ru },
	locale: localStorage.language || navigator.language.split('-')[0] || 'en',
});

const vuetify = new Vuetify({ 
	icons: { iconfont: 'md' },
	lang: { t: ( key, ...params ) => i18n.t( key, params ), },
	theme: { 
		dark: true,
		themes: {
			dark: {
				primary: '#cc7733',
			}
		},
	},
});

const store = new Vuex.Store({
	state: {
		toast: null,
	},
	mutations: {
		showToast: function( state, { color, message } ) {
			state.toast = { color, message };
		},
		bindModule: ( _, { module, name } ) => {
			Vue.set( module.state, '_namespace', name.endsWith('/') ? name.slice(0, -1) : name );
		}
	},
	actions: {
		bindModules: function( { commit }, { _modulesNamespaceMap } ) {
			Object.entries( _modulesNamespaceMap ).forEach( ([name, module]) => {
				commit('bindModule', { module, name } );
			});
		},
	},
	modules: {
		menu: dataProvider,
		keys: dataProvider,
	}
});

const router = new VueRouter({
	routes: [
		{
			path: '/',
			component: frame,
		},
		{
			path: '*', redirect: '/'
		},
	]
});

new Vue({ 
	el: '#root',
	template: '#app',
	router: router,
	i18n: i18n,
	store: store,
	vuetify: vuetify,
	data: () => ({
		snackbar: {
			shown: false,
			color: null,
			text: null,
		},
	}),
	created: function() {
		this.$store.dispatch('bindModules', this.$store);
	},
	watch: {
		'$store.state.toast': function( { color, message } ) {
			this.snackbar.shown = true;
			this.snackbar.color = color;
			this.snackbar.text = message;
		},
	}
});
